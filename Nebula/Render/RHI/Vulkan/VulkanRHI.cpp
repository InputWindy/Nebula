#include "VulkanRHI.h"
#include <vulkan/vulkan.h>

#include <stdexcept>

#include <iostream>

#include "RHI/RHI.h"
#include "RenderCore.h"

#include <GLFW/glfw3.h>
#include <optional>
#include <string>
#include <map>
#include <set>


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
	/*
		卸载校验层
	*/
	/*if (Info.bEnableValidationLayers)
	{
		PFN_vkDestroyDebugUtilsMessengerEXT DestroyDebugUtilsMessengerFunc = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(Instance, "vkDestroyDebugUtilsMessengerEXT");
		if (DestroyDebugUtilsMessengerFunc != nullptr) 
			DestroyDebugUtilsMessengerFunc(Instance, DebugMessenger, nullptr);
		
	}*/
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

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) 
{
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) 
{
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, debugMessenger, pAllocator);
	}
}

void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = DebugCallback;
}

bool CheckInstanceLayerSupport(const std::vector<const char*>& validationLayers)
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : validationLayers) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}
	}

	return true;
}

std::vector<const char*> GetSurfaceRequiredInstanceExtensions() 
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	return extensions;
}

bool CheckDeviceExtensionSupport(VkPhysicalDevice device,const std::vector<const char*>& deviceExtensions)
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for (const auto& extension : availableExtensions) 
	{
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device,VkSurfaceKHR surface) 
{
	SwapChainSupportDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

	if (formatCount != 0) {
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

	if (presentModeCount != 0) {
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
	}

	return details;
}

nbl::nEnumRHIInitResult nbl::nVulkanRHI::InitBackend(nRHICreateInfo*NewInfo)
{
	nbl::nEnumRHIInitResult Result = nRHI::InitBackend(NewInfo);

	if (Result != nEnumRHIInitResult::Success)return Result;

#pragma region 整理需要的硬件特性
	/*
		实例级Layer、Ext + 设备级Ext
	*/
	std::vector<const char*> InstanceLayers;
	std::vector<const char*> InstanceEXTs;
	std::vector<const char*> DeviceEXTs;

	// 开启校验层和校验层需要的实例级拓展
	if (NewInfo->bEnableValidationLayer)
	{
		InstanceLayers.push_back("VK_LAYER_KHRONOS_validation");
		InstanceEXTs.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	bool bNeedPresent = false;
	bool bNeedGraphic = false;
	bool bNeedCompute = false;

	// 判断是否要开启Surface拓展
	switch (NewInfo->SurfaceType)
	{
	case nbl::nEnumSurfaceType::OffScreen:
	{
		bNeedPresent = false;
		break;
	}
	case nbl::nEnumSurfaceType::Windowed:
	{
		for (const char* Ext : GetSurfaceRequiredInstanceExtensions())
		{
			InstanceEXTs.push_back(Ext);
		}

		DeviceEXTs.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

		bNeedPresent = true;

		break;
	}
	default:
		break;
	}
#pragma endregion

	/*
		检查硬件Vulkan sdk是否支持实例层
	*/
	if(!CheckInstanceLayerSupport(InstanceLayers))
		return nEnumRHIInitResult::UnavailableLayers;
	else
	{
		/*
			创建Vk实例
		*/
		VkInstanceCreateInfo InstanceCreateInfo;
		{
			VkApplicationInfo AppInfo{};
			AppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			AppInfo.pApplicationName = NewInfo->AppName.c_str();
			AppInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
			AppInfo.pEngineName = "No Engine";
			AppInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);

			switch (NewInfo->FeatureLevel)
			{
			case nbl::nEnumRenderFeatureLevel::GraphicOnly:
			{
				AppInfo.apiVersion = VK_API_VERSION_1_0;
				bNeedGraphic = true;
				break;
			}
			case nbl::nEnumRenderFeatureLevel::GraphicAndCompute:
			{
				AppInfo.apiVersion = VK_API_VERSION_1_0;
				bNeedGraphic = true;
				bNeedCompute = true;
				break;
			}
			case nbl::nEnumRenderFeatureLevel::PC:
			{
				AppInfo.apiVersion = VK_API_VERSION_1_3;
				bNeedGraphic = true;
				bNeedCompute = true;
				break;
			}
			case nbl::nEnumRenderFeatureLevel::Mobile:
			{
				AppInfo.apiVersion = VK_API_VERSION_1_0;
				bNeedGraphic = true;
				bNeedCompute = true;
				break;
			}
			default:
				break;
			}

			InstanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			InstanceCreateInfo.pApplicationInfo = &AppInfo;
			InstanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(InstanceEXTs.size());
			InstanceCreateInfo.ppEnabledExtensionNames = InstanceEXTs.data();

			InstanceCreateInfo.enabledLayerCount = 0;
			InstanceCreateInfo.pNext = nullptr;

			if (NewInfo->bEnableValidationLayer) 
			{
				VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
				InstanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(InstanceLayers.size());
				InstanceCreateInfo.ppEnabledLayerNames = InstanceLayers.data();

				PopulateDebugMessengerCreateInfo(debugCreateInfo);
				InstanceCreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
			}
		}

		if (vkCreateInstance(&InstanceCreateInfo, nullptr, &Instance) != VK_SUCCESS) 
			return nEnumRHIInitResult::InvalidVkInstance;
		else
		{
			/*
				创建实例成功后
			*/
			if (NewInfo->bEnableValidationLayer)
			{
				VkDebugUtilsMessengerCreateInfoEXT createInfo;
				PopulateDebugMessengerCreateInfo(createInfo);

				if (CreateDebugUtilsMessengerEXT(Instance, &createInfo, nullptr, &DebugMessenger) != VK_SUCCESS) 
					return nEnumRHIInitResult::InvalidDebugUtils;
			}

			/*
				创建surface,需要呈现到窗口上，就必须要有surface
			*/
			if (bNeedPresent)
			{
				if (glfwCreateWindowSurface(Instance, (GLFWwindow*)(NewInfo->PlatformWindow.GetHandle()), nullptr, &Surface) != VK_SUCCESS)
					return nEnumRHIInitResult::InvalidSurface;
			}

			/*
				获取物理设备
			*/
			{
				uint32_t deviceCount = 0;
				vkEnumeratePhysicalDevices(Instance, &deviceCount, nullptr);

				if (deviceCount == 0)
					return nEnumRHIInitResult::NoSuitableGPU;
				else
				{
					std::vector<VkPhysicalDevice> devices(deviceCount);
					vkEnumeratePhysicalDevices(Instance, &deviceCount, devices.data());

					/*
						查询所有可用的物理设备
					*/
					for (const auto& device : devices) 
					{
						if (CheckDeviceExtensionSupport(device, DeviceEXTs))
						{
							/*
								检查是否支持Compute shader并行计算
							*/
							uint32_t queueFamilyCount = 0;
							vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

							std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
							vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

							bool bSuitable = false;

							std::optional<uint32_t> graphicsFamily;
							std::optional<uint32_t> presentFamily;
							std::optional<uint32_t> computeFamily;

							for (int i = 0; i < queueFamilies.size(); ++i)
							{
								VkQueueFamilyProperties& queueFamily = queueFamilies[i];

								if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
									graphicsFamily = i;

								if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT)
									computeFamily = i;


								VkBool32 presentSupport = false;
								vkGetPhysicalDeviceSurfaceSupportKHR(device, i, Surface, &presentSupport);

								if (presentSupport)
								{
									/*
										还要检查呈现模式是否匹配
									*/
									SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device, Surface);

									if (!swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty())
										presentFamily = i;
								}

								bSuitable = true;
								bSuitable &= bNeedGraphic ? graphicsFamily.has_value() : true;
								bSuitable &= bNeedCompute ? computeFamily.has_value() : true;
								bSuitable &= bNeedPresent ? presentFamily.has_value() : true;

								if (bSuitable)
									break;
									
							};

							if (bSuitable)
								SuitablePhysicalDevices.push_back(device);
						}
					}

					/*
						取一个可用的物理设备
					*/
					if (SuitablePhysicalDevices.size())
						PhysicalDevice = SuitablePhysicalDevices[0];

					if (PhysicalDevice == VK_NULL_HANDLE) 
						return nEnumRHIInitResult::NoSuitableGPU;
				}

				
			}


		}
	}


	return nEnumRHIInitResult::Success;
}

extern "C" const char** glfwGetRequiredInstanceExtensions(uint32_t*);

//nbl::nEnumRHIInitResult nbl::nVulkanRHI::InitBackend(nbl::nRHICreateInfo* NewInfo)
//{
//	nEnumRHIInitResult Result = nRHI::InitBackend(NewInfo);
//	if(Result != nEnumRHIInitResult::Success)return Result;
//
//	// check layer available
//	if (Info.bEnableValidationLayers && !IsSupportedLayer(Info.ValidationLayers))
//		return nEnumRHIInitResult::InavailableLayers;
//
//	if (
//		!Info.PlatformWindow.IsValid() ||
//		Info.PlatformWindow.GetInfo().RenderBackend != nEnumWindowBackend::Glfw
//		)
//		return nEnumRHIInitResult::InvalidPlatformWindow;
//
//	auto GetRequiredExtensions = [](const nRHICreateInfo& Info)->std::vector<const char*>
//		{
//			uint32_t glfwExtensionCount = 0;
//			const char** glfwExtensions;
//			glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
//
//			std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
//
//			/*
//				这个validation layer需要用到vulkan的VK_EXT_debug_utils拓展
//			*/
//			if (Info.bEnableValidationLayers) 
//				extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
//			
//			return extensions;
//		};
//
//	VkApplicationInfo AppInfo{};
//	AppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
//	AppInfo.pApplicationName = Info.AppName.c_str();
//	AppInfo.applicationVersion = Info.AppVersion;
//	AppInfo.pEngineName = Info.EngineName.c_str();
//	AppInfo.engineVersion = Info.EngineVersion;
//	AppInfo.apiVersion = Info.VulkanVersion;
//
//	VkInstanceCreateInfo CreateInfo{};
//	CreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
//	CreateInfo.pApplicationInfo = &AppInfo;
//
//	/*
//		vk只提供最小功能实现，很多额外功能都是以拓展形式实现的，这里就是配置vulkan的窗口拓展（VK_KHR_swapchain）。
//		类似的拓展还有VK_KHR_ray_tracing 支持光追、VK_KHR_multiview 支持VR/AR
//
//		拓展可以配给整个Instance，也可以只配给具体的一个PhysicalDevice
//	*/
//	auto Extensions = GetRequiredExtensions(Info);
//	CreateInfo.enabledExtensionCount = static_cast<uint32_t>(Extensions.size());
//	CreateInfo.ppEnabledExtensionNames = Extensions.data();
//
//	//开启Layer。Layer相当于在Vulkan代码里嵌入代码，主要是给RenderDoc这类代码分析工具用的。
//	VkDebugUtilsMessengerCreateInfoEXT DebugCreateInfo{};
//	
//	if (Info.bEnableValidationLayers) 
//	{
//		CreateInfo.enabledLayerCount = static_cast<uint32_t>(Info.ValidationLayers.size());
//		CreateInfo.ppEnabledLayerNames = Info.ValidationLayers.data();
//
//		DebugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
//		DebugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
//		DebugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
//		DebugCreateInfo.pfnUserCallback = DebugCallback;
//		CreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&DebugCreateInfo;
//	}
//	else {
//		CreateInfo.enabledLayerCount = 0;
//		CreateInfo.pNext = nullptr;
//	}
//
//	/*
//		创建VkInstance
//	*/
//	if (vkCreateInstance(&CreateInfo, nullptr, &Instance) != VK_SUCCESS) 
//	{
//		throw std::runtime_error("failed to create instance!");
//	}
//	/*
//		Debug模式下创建Debug层
//	*/
//	if (Info.bEnableValidationLayers)
//		if (PFN_vkCreateDebugUtilsMessengerEXT DebugUtilsMessengerCreator = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(Instance, "vkCreateDebugUtilsMessengerEXT"))
//		{
//			if(DebugUtilsMessengerCreator(Instance, &DebugCreateInfo, nullptr, &DebugMessenger) != VK_SUCCESS) 
//				throw std::runtime_error("failed to set up debug messenger!");
//			else
//				std::cout << "Debug Utils Created Successfully" << std::endl;
//		}
//
//	/*
//		创建Surface
//	*/
//	if (glfwCreateWindowSurface(Instance, (GLFWwindow*)Info.PlatformWindow.GetHandle(), nullptr, &Surface) != VK_SUCCESS)
//		throw std::runtime_error("failed to create window surface!");
//	else
//		std::cout << "Surface Created Successfully" << std::endl;
//	
//	auto FindQueueFamilies = [this](VkPhysicalDevice device)-> QueueFamilyIndices
//		{
//			QueueFamilyIndices indices;
//
//			uint32_t queueFamilyCount = 0;
//			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
//
//			std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
//			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
//
//			int i = 0;
//			for (const auto& queueFamily : queueFamilies) {
//				if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
//					indices.graphicsFamily = i;
//				}
//
//				VkBool32 presentSupport = false;
//				vkGetPhysicalDeviceSurfaceSupportKHR(device, i, Surface, &presentSupport);
//
//				if (presentSupport) {
//					indices.presentFamily = i;
//				}
//
//				if (indices.isComplete()) {
//					break;
//				}
//
//				i++;
//			};
//
//			return indices;
//		};
//
//	const std::vector<const char*> DeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
//	/*
//		查询可用的物理设备
//	*/
//	{
//		
//
//		uint32_t DeviceCount = 0;
//		vkEnumeratePhysicalDevices(Instance, &DeviceCount, nullptr);
//
//		PhysicalDevices.resize(DeviceCount);
//
//		if (DeviceCount == 0)
//			throw std::runtime_error("failed to find GPUs with Vulkan support!");
//
//		vkEnumeratePhysicalDevices(Instance, &DeviceCount, PhysicalDevices.data());
//
//		auto CheckDeviceExtensionSupport = [this,&DeviceExtensions](VkPhysicalDevice device)->bool
//			{
//				uint32_t extensionCount;
//				vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
//
//				std::vector<VkExtensionProperties> availableExtensions(extensionCount);
//				vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());
//
//				std::set<std::string> requiredExtensions(DeviceExtensions.begin(), DeviceExtensions.end());
//
//				for (const auto& extension : availableExtensions) {
//					requiredExtensions.erase(extension.extensionName);
//				}
//
//				return requiredExtensions.empty();
//			};
//
//		auto QuerySwapChainSupport = [this](VkPhysicalDevice device)->SwapChainSupportDetails
//			{
//				SwapChainSupportDetails details;
//
//				vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, Surface, &details.capabilities);
//
//				uint32_t formatCount;
//				vkGetPhysicalDeviceSurfaceFormatsKHR(device, Surface, &formatCount, nullptr);
//
//				if (formatCount != 0) {
//					details.formats.resize(formatCount);
//					vkGetPhysicalDeviceSurfaceFormatsKHR(device, Surface, &formatCount, details.formats.data());
//				}
//
//				uint32_t presentModeCount;
//				vkGetPhysicalDeviceSurfacePresentModesKHR(device, Surface, &presentModeCount, nullptr);
//
//				if (presentModeCount != 0) {
//					details.presentModes.resize(presentModeCount);
//					vkGetPhysicalDeviceSurfacePresentModesKHR(device, Surface, &presentModeCount, details.presentModes.data());
//				}
//
//				return details;
//			};
//
//		auto IsDeviceSuitable = [this,&FindQueueFamilies,&CheckDeviceExtensionSupport,&QuerySwapChainSupport](VkPhysicalDevice device)->bool
//			{
//				QueueFamilyIndices indices = FindQueueFamilies(device);
//
//				bool extensionsSupported = CheckDeviceExtensionSupport(device);
//
//				bool swapChainAdequate = false;
//				if (extensionsSupported) {
//					SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device);
//					swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
//				}
//
//				return indices.isComplete() && extensionsSupported && swapChainAdequate;
//			};
//
//		for (const auto& Iter : PhysicalDevices)
//		{
//			if (IsDeviceSuitable(Iter))
//			{
//				PhysicalDevice = Iter;
//				break;
//			}
//		}
//
//		if (PhysicalDevice == VK_NULL_HANDLE)
//			throw std::runtime_error("failed to find a suitable GPU!");
//		else
//			std::cout << "find suitable SPU!" << std::endl;
//	}
//
//	/*
//		创建逻辑设备
//	*/
//	{
//		QueueFamilyIndices indices = FindQueueFamilies(PhysicalDevice);
//
//		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
//		std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };
//
//		float queuePriority = 1.0f;
//		for (uint32_t queueFamily : uniqueQueueFamilies) {
//			VkDeviceQueueCreateInfo queueCreateInfo{};
//			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
//			queueCreateInfo.queueFamilyIndex = queueFamily;
//			queueCreateInfo.queueCount = 1;
//			queueCreateInfo.pQueuePriorities = &queuePriority;
//			queueCreateInfos.push_back(queueCreateInfo);
//		}
//
//		VkPhysicalDeviceFeatures deviceFeatures{};
//
//		VkDeviceCreateInfo createInfo{};
//		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
//
//		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
//		createInfo.pQueueCreateInfos = queueCreateInfos.data();
//
//		createInfo.pEnabledFeatures = &deviceFeatures;
//
//		createInfo.enabledExtensionCount = static_cast<uint32_t>(DeviceExtensions.size());
//		createInfo.ppEnabledExtensionNames = DeviceExtensions.data();
//
//		if (Info.bEnableValidationLayers) 
//		{
//			createInfo.enabledLayerCount = static_cast<uint32_t>(Info.ValidationLayers.size());
//			createInfo.ppEnabledLayerNames = Info.ValidationLayers.data();
//		}
//		else {
//			createInfo.enabledLayerCount = 0;
//		}
//
//		if (vkCreateDevice(PhysicalDevice, &createInfo, nullptr, &LogicDevice) != VK_SUCCESS)
//			throw std::runtime_error("failed to create logical device!");
//		else
//			std::cout << "create logic device ." << std::endl;
//
//		vkGetDeviceQueue(LogicDevice, indices.graphicsFamily.value(), 0, &GraphicsQueue);
//		vkGetDeviceQueue(LogicDevice, indices.presentFamily.value(), 0, &PresentQueue);
//	}
//	
//	return nEnumRHIInitResult::Success;
//}

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
