#include "VulkanRHI.h"
#include <vulkan/vulkan.h>

#include <stdexcept>

#include <iostream>

#include "RHI/RHI.h"

#include <GLFW/glfw3.h>
#include <optional>
#include <string>
#include <map>
#include <set>

struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() 
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

struct SwapChainSupportDetails 
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

nbl::nVulkanRHI::nVulkanRHI()
{
	uint32_t LayerCount = 0;
	vkEnumerateInstanceLayerProperties(&LayerCount, nullptr);
	AvailableLayers.resize(LayerCount);
	vkEnumerateInstanceLayerProperties(&LayerCount, AvailableLayers.data());
}

nbl::nVulkanRHI::~nVulkanRHI()
{
}

nbl::nEnumRenderBackend nbl::nVulkanRHI::GetType() const
{
	return nEnumRenderBackend::Vulkan;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) 
{
	std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

	return VK_FALSE;
}

extern "C" const char** glfwGetRequiredInstanceExtensions(uint32_t*);

nbl::nEnumRHIInitResult nbl::nVulkanRHI::InitBackend(nbl::nRHICreateInfo* NewInfo)
{
	nEnumRHIInitResult Result = nRHI::InitBackend(NewInfo);
	if(Result != nEnumRHIInitResult::Success)return Result;

	// check layer available
	if (Info.bEnableValidationLayers && !IsSupportedLayer(Info.ValidationLayers))
		return nEnumRHIInitResult::InavailableLayers;

	if (
		!Info.PlatformWindow.IsValid() ||
		Info.PlatformWindow.GetType() != nEnumWindowBackend::Glfw
		)
		return nEnumRHIInitResult::InvalidPlatformWindow;

	auto GetRequiredExtensions = [](const nRHICreateInfo& Info)->std::vector<const char*>
		{
			uint32_t glfwExtensionCount = 0;
			const char** glfwExtensions;
			glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

			std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

			if (Info.bEnableValidationLayers) 
				extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
			
			return extensions;
		};

	VkApplicationInfo AppInfo{};
	AppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	AppInfo.pApplicationName = Info.AppName.c_str();
	AppInfo.applicationVersion = Info.AppVersion;
	AppInfo.pEngineName = Info.EngineName.c_str();
	AppInfo.engineVersion = Info.EngineVersion;
	AppInfo.apiVersion = Info.VulkanVersion;

	VkInstanceCreateInfo CreateInfo{};
	CreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	CreateInfo.pApplicationInfo = &AppInfo;

	/*
		vk只提供最小功能实现，很多额外功能都是以拓展形式实现的，这里就是配置vulkan的窗口拓展（VK_KHR_swapchain）。
		类似的拓展还有VK_KHR_ray_tracing 支持光追、VK_KHR_multiview 支持VR/AR

		拓展可以配给整个Instance，也可以只配给具体的一个PhysicalDevice
	*/
	auto Extensions = GetRequiredExtensions(Info);
	CreateInfo.enabledExtensionCount = static_cast<uint32_t>(Extensions.size());
	CreateInfo.ppEnabledExtensionNames = Extensions.data();

	//开启Layer。Layer相当于在Vulkan代码里嵌入代码，主要是给RenderDoc这类代码分析工具用的。
	VkDebugUtilsMessengerCreateInfoEXT DebugCreateInfo{};
	
	if (Info.bEnableValidationLayers) 
	{
		CreateInfo.enabledLayerCount = static_cast<uint32_t>(Info.ValidationLayers.size());
		CreateInfo.ppEnabledLayerNames = Info.ValidationLayers.data();

		DebugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		DebugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		DebugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		DebugCreateInfo.pfnUserCallback = DebugCallback;
		CreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&DebugCreateInfo;
	}
	else {
		CreateInfo.enabledLayerCount = 0;
		CreateInfo.pNext = nullptr;
	}

	/*
		创建VkInstance
	*/
	if (vkCreateInstance(&CreateInfo, nullptr, &Instance) != VK_SUCCESS) 
	{
		throw std::runtime_error("failed to create instance!");
	}
	/*
		Debug模式下创建Debug层
	*/
	if (Info.bEnableValidationLayers)
		if (PFN_vkCreateDebugUtilsMessengerEXT DebugUtilsMessengerCreator = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(Instance, "vkCreateDebugUtilsMessengerEXT"))
		{
			if(DebugUtilsMessengerCreator(Instance, &DebugCreateInfo, nullptr, &DebugMessenger) != VK_SUCCESS) 
				throw std::runtime_error("failed to set up debug messenger!");
			else
				std::cout << "Debug Utils Created Successfully" << std::endl;
		}

	/*
		创建Surface
	*/
	if (glfwCreateWindowSurface(Instance, (GLFWwindow*)Info.PlatformWindow.GetHandle(), nullptr, &Surface) != VK_SUCCESS)
		throw std::runtime_error("failed to create window surface!");
	else
		std::cout << "Surface Created Successfully" << std::endl;
	
	auto FindQueueFamilies = [this](VkPhysicalDevice device)-> QueueFamilyIndices
		{
			QueueFamilyIndices indices;

			uint32_t queueFamilyCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

			std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

			int i = 0;
			for (const auto& queueFamily : queueFamilies) {
				if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
					indices.graphicsFamily = i;
				}

				VkBool32 presentSupport = false;
				vkGetPhysicalDeviceSurfaceSupportKHR(device, i, Surface, &presentSupport);

				if (presentSupport) {
					indices.presentFamily = i;
				}

				if (indices.isComplete()) {
					break;
				}

				i++;
			};

			return indices;
		};

	const std::vector<const char*> DeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	/*
		查询可用的物理设备
	*/
	{
		

		uint32_t DeviceCount = 0;
		vkEnumeratePhysicalDevices(Instance, &DeviceCount, nullptr);

		PhysicalDevices.resize(DeviceCount);

		if (DeviceCount == 0)
			throw std::runtime_error("failed to find GPUs with Vulkan support!");

		vkEnumeratePhysicalDevices(Instance, &DeviceCount, PhysicalDevices.data());

		auto CheckDeviceExtensionSupport = [this,&DeviceExtensions](VkPhysicalDevice device)->bool
			{
				uint32_t extensionCount;
				vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

				std::vector<VkExtensionProperties> availableExtensions(extensionCount);
				vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

				std::set<std::string> requiredExtensions(DeviceExtensions.begin(), DeviceExtensions.end());

				for (const auto& extension : availableExtensions) {
					requiredExtensions.erase(extension.extensionName);
				}

				return requiredExtensions.empty();
			};

		auto QuerySwapChainSupport = [this](VkPhysicalDevice device)->SwapChainSupportDetails
			{
				SwapChainSupportDetails details;

				vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, Surface, &details.capabilities);

				uint32_t formatCount;
				vkGetPhysicalDeviceSurfaceFormatsKHR(device, Surface, &formatCount, nullptr);

				if (formatCount != 0) {
					details.formats.resize(formatCount);
					vkGetPhysicalDeviceSurfaceFormatsKHR(device, Surface, &formatCount, details.formats.data());
				}

				uint32_t presentModeCount;
				vkGetPhysicalDeviceSurfacePresentModesKHR(device, Surface, &presentModeCount, nullptr);

				if (presentModeCount != 0) {
					details.presentModes.resize(presentModeCount);
					vkGetPhysicalDeviceSurfacePresentModesKHR(device, Surface, &presentModeCount, details.presentModes.data());
				}

				return details;
			};

		auto IsDeviceSuitable = [this,&FindQueueFamilies,&CheckDeviceExtensionSupport,&QuerySwapChainSupport](VkPhysicalDevice device)->bool
			{
				QueueFamilyIndices indices = FindQueueFamilies(device);

				bool extensionsSupported = CheckDeviceExtensionSupport(device);

				bool swapChainAdequate = false;
				if (extensionsSupported) {
					SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device);
					swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
				}

				return indices.isComplete() && extensionsSupported && swapChainAdequate;
			};

		for (const auto& Iter : PhysicalDevices)
		{
			if (IsDeviceSuitable(Iter))
			{
				PhysicalDevice = Iter;
				break;
			}
		}

		if (PhysicalDevice == VK_NULL_HANDLE)
			throw std::runtime_error("failed to find a suitable GPU!");
		else
			std::cout << "find suitable SPU!" << std::endl;
	}

	/*
		创建逻辑设备
	*/
	{
		QueueFamilyIndices indices = FindQueueFamilies(PhysicalDevice);

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		float queuePriority = 1.0f;
		for (uint32_t queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures{};

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();

		createInfo.pEnabledFeatures = &deviceFeatures;

		createInfo.enabledExtensionCount = static_cast<uint32_t>(DeviceExtensions.size());
		createInfo.ppEnabledExtensionNames = DeviceExtensions.data();

		if (Info.bEnableValidationLayers) 
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(Info.ValidationLayers.size());
			createInfo.ppEnabledLayerNames = Info.ValidationLayers.data();
		}
		else {
			createInfo.enabledLayerCount = 0;
		}

		if (vkCreateDevice(PhysicalDevice, &createInfo, nullptr, &LogicDevice) != VK_SUCCESS)
			throw std::runtime_error("failed to create logical device!");
		else
			std::cout << "create logic device ." << std::endl;

		vkGetDeviceQueue(LogicDevice, indices.graphicsFamily.value(), 0, &GraphicsQueue);
		vkGetDeviceQueue(LogicDevice, indices.presentFamily.value(), 0, &PresentQueue);
	}
	
	return nEnumRHIInitResult::Success;
}

const auto& nbl::nVulkanRHI::GetCreateInfo() const
{
	return Info;
}

const auto& nbl::nVulkanRHI::GetAvailableLayers() const
{
	return AvailableLayers;
}

bool nbl::nVulkanRHI::IsSupportedLayer(const std::vector<const char*>& ValidationLayers) const
{
	for (const char* LayerName : ValidationLayers) 
	{
		bool bLayerFound = false;

		for (const auto& LayerProperties : GetAvailableLayers()) 
		{
			if (strcmp(LayerName, LayerProperties.layerName) == 0)
			{
				bLayerFound = true;
				break;
			}
		}

		if (!bLayerFound)
			return false;
		
	}

	return true;
}
