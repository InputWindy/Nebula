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


	DestroyInstance();
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

VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) 
{
	for (const auto& availableFormat : availableFormats) {
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return availableFormat;
		}
	}

	return availableFormats[0];
}

VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) 
{
	for (const auto& availablePresentMode : availablePresentModes) {
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			return availablePresentMode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

#include <algorithm>

VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window)
{
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		return capabilities.currentExtent;
	}
	else 
	{
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		VkExtent2D actualExtent = {
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
		};

		actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return actualExtent;
	}
}

nbl::nEnumRHIInitResult nbl::nVulkanRHI::InitBackend(nRHICreateInfo*NewInfo)
{
	nbl::nEnumRHIInitResult Result = nRHI::InitBackend(NewInfo);

	if (Result != nEnumRHIInitResult::Success)return Result;

	/*
		检查要开哪些硬件特性
	*/
	PopulateRequirements();

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
		if (CreateInsatnce() != VK_SUCCESS)
			return nEnumRHIInitResult::InvalidVkInstance;
		else
		{
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
						查询所有可用的物理设备（是否支持引擎特性所需的相应队列）
					*/
					for (const auto& device : devices) 
					{
						if (CheckDeviceExtensionSupport(device, DeviceEXTs))
						{
							std::optional<uint32_t> graphicsFamily;
							std::optional<uint32_t> presentFamily;
							std::optional<uint32_t> computeFamily;

							/*
								检查是否支持Compute shader并行计算
							*/
							uint32_t queueFamilyCount = 0;
							vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

							std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
							vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

							bool bSuitable = false;

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

					/*
						从物理设备配置一个逻辑设备
					*/
					VkPhysicalDeviceFeatures deviceFeatures{};
					std::vector<VkDeviceQueueCreateInfo> QueueCreateInfos;
					VkDeviceCreateInfo DeviceCreateInfo{};

					/*
						把设备队列拿出来
					*/
					std::optional<uint32_t> computeFamily;
					std::optional<uint32_t> graphicsFamily;
					std::optional<uint32_t> presentFamily;
					{
						std::set<uint32_t> UniqueQueueFamilies;

						uint32_t queueFamilyCount = 0;
						vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDevice, &queueFamilyCount, nullptr);

						std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
						vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDevice, &queueFamilyCount, queueFamilies.data());

						for (int i = 0; i < queueFamilies.size(); ++i)
						{
							VkQueueFamilyProperties& queueFamily = queueFamilies[i];

							if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
								graphicsFamily = i;

							if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT)
								computeFamily = i;


							VkBool32 presentSupport = false;
							vkGetPhysicalDeviceSurfaceSupportKHR(PhysicalDevice, i, Surface, &presentSupport);

							if (presentSupport)
							{
								/*
									还要检查呈现模式是否匹配
								*/
								SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(PhysicalDevice, Surface);

								if (!swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty())
									presentFamily = i;
							}

							bool bSuitable = true;
							bSuitable &= bNeedGraphic ? graphicsFamily.has_value() : true;
							bSuitable &= bNeedCompute ? computeFamily.has_value() : true;
							bSuitable &= bNeedPresent ? presentFamily.has_value() : true;

							if (bSuitable)
								break;

						};

						if (graphicsFamily.has_value())UniqueQueueFamilies.insert(graphicsFamily.value());
						if (presentFamily.has_value())UniqueQueueFamilies.insert(presentFamily.value());
						if (computeFamily.has_value())UniqueQueueFamilies.insert(computeFamily.value());

						float queuePriority = 1.0f;
						for (uint32_t queueFamily : UniqueQueueFamilies)
						{
							VkDeviceQueueCreateInfo queueCreateInfo{};
							queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
							queueCreateInfo.queueFamilyIndex = queueFamily;
							queueCreateInfo.queueCount = 1;
							queueCreateInfo.pQueuePriorities = &queuePriority;
							QueueCreateInfos.push_back(queueCreateInfo);
						}
					}
					{
						DeviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

						DeviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(QueueCreateInfos.size());
						DeviceCreateInfo.pQueueCreateInfos = QueueCreateInfos.data();

						DeviceCreateInfo.pEnabledFeatures = &deviceFeatures;

						DeviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(DeviceEXTs.size());
						DeviceCreateInfo.ppEnabledExtensionNames = DeviceEXTs.data();
						DeviceCreateInfo.enabledLayerCount = 0;
						DeviceCreateInfo.ppEnabledLayerNames = nullptr;

						if (NewInfo->bEnableValidationLayer)
						{
							DeviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(InstanceLayers.size());
							DeviceCreateInfo.ppEnabledLayerNames = InstanceLayers.data();
						}
					}

					if (vkCreateDevice(PhysicalDevice, &DeviceCreateInfo, nullptr, &LogicDevice) != VK_SUCCESS)
						return nEnumRHIInitResult::InvalidLogicDevice;

					/*
						从逻辑设备里取出来队列
					*/
					if (bNeedCompute && computeFamily.has_value()) vkGetDeviceQueue(LogicDevice, computeFamily.value(), 0, &ComputeQueue);
					if (bNeedGraphic && graphicsFamily.has_value()) vkGetDeviceQueue(LogicDevice, graphicsFamily.value(), 0, &GraphicsQueue);
					if (bNeedPresent && presentFamily.has_value()) vkGetDeviceQueue(LogicDevice, presentFamily.value(), 0, &PresentQueue);

					/*
						如果有呈现队列，就创建交换链
					*/
					if (bNeedPresent && bNeedGraphic)
					{
						SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(PhysicalDevice, Surface);

						VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
						VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
						VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities, (GLFWwindow*)(NewInfo->PlatformWindow.GetHandle()));

						uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
						if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
						{
							imageCount = swapChainSupport.capabilities.maxImageCount;
						}

						VkSwapchainCreateInfoKHR createInfo{};
						createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
						createInfo.surface = Surface;

						createInfo.minImageCount = imageCount;/*image count和n缓冲相对应*/
						createInfo.imageFormat = surfaceFormat.format;
						createInfo.imageColorSpace = surfaceFormat.colorSpace;
						createInfo.imageExtent = extent;
						createInfo.imageArrayLayers = 1;
						createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
						/*交换链创建的Image支持可以作为帧缓冲的颜色输出、也支持离屏渲染image拷贝过来显示*/

						uint32_t queueFamilyIndices[] = { graphicsFamily.value(), presentFamily.value() };

						/*
							如果queue同时支持显示和图形功能，就设置为share模式，否则就分开
						*/
						if (graphicsFamily != presentFamily) 
						{
							createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
							createInfo.queueFamilyIndexCount = 2;
							createInfo.pQueueFamilyIndices = queueFamilyIndices;
						}
						else 
						{
							createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
						}

						createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
						createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
						createInfo.presentMode = presentMode;
						createInfo.clipped = VK_TRUE;

						if (vkCreateSwapchainKHR(LogicDevice, &createInfo, nullptr, &SwapChain) != VK_SUCCESS)
							return nEnumRHIInitResult::InvalidSwapChain;

						vkGetSwapchainImagesKHR(LogicDevice, SwapChain, &imageCount, nullptr);
						SwapChainImages.resize(imageCount);
						vkGetSwapchainImagesKHR(LogicDevice, SwapChain, &imageCount, SwapChainImages.data());

						SwapChainImageFormat = surfaceFormat.format;
						SwapChainExtent = extent;

						SwapChainImageViews.resize(SwapChainImages.size());

						for (size_t i = 0; i < SwapChainImages.size(); i++) 
						{
							VkImageViewCreateInfo createInfo{};
							createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
							createInfo.image = SwapChainImages[i];
							createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
							createInfo.format = SwapChainImageFormat;
							createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
							createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
							createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
							createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
							createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
							createInfo.subresourceRange.baseMipLevel = 0;
							createInfo.subresourceRange.levelCount = 1;
							createInfo.subresourceRange.baseArrayLayer = 0;
							createInfo.subresourceRange.layerCount = 1;

							if (vkCreateImageView(LogicDevice, &createInfo, nullptr, &SwapChainImageViews[i]) != VK_SUCCESS) 
								return nEnumRHIInitResult::InvalidImageViews;
						}
					}
				}
			}
		}
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

void nbl::nVulkanRHI::PopulateRequirements()
{

#pragma region 整理需要的硬件特性

	// 开启校验层和校验层需要的实例级拓展
	if (Info.bEnableValidationLayer)
	{
		InstanceLayers.push_back("VK_LAYER_KHRONOS_validation");
		InstanceEXTs.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	// 判断是否要开启Surface拓展
	switch (Info.SurfaceType)
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

	switch (Info.FeatureLevel)
	{
	case nbl::nEnumRenderFeatureLevel::GraphicOnly:
	{
		bNeedGraphic = true;
		break;
	}
	case nbl::nEnumRenderFeatureLevel::ComputeOnly:
	{
		bNeedCompute = true;
		break;
	}
	case nbl::nEnumRenderFeatureLevel::GraphicAndCompute:
	{
		bNeedGraphic = true;
		bNeedCompute = true;
		break;
	}
	case nbl::nEnumRenderFeatureLevel::PC:
	{
		bNeedGraphic = true;
		bNeedCompute = true;
		break;
	}
	case nbl::nEnumRenderFeatureLevel::Mobile:
	{
		bNeedGraphic = true;
		bNeedCompute = true;
		break;
	}
	default:
		break;
	}
#pragma endregion
}

VkResult nbl::nVulkanRHI::CreateInsatnce()
{
	VkInstanceCreateInfo InstanceCreateInfo;
	{
		VkApplicationInfo AppInfo{};
		AppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		AppInfo.pApplicationName = Info.AppName.c_str();
		AppInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		AppInfo.pEngineName = "No Engine";
		AppInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);

		switch (Info.FeatureLevel)
		{
		case nbl::nEnumRenderFeatureLevel::GraphicOnly:
		{
			AppInfo.apiVersion = VK_API_VERSION_1_0;
			break;
		}
		case nbl::nEnumRenderFeatureLevel::ComputeOnly:
		{
			AppInfo.apiVersion = VK_API_VERSION_1_0;
			break;
		}
		case nbl::nEnumRenderFeatureLevel::GraphicAndCompute:
		{
			AppInfo.apiVersion = VK_API_VERSION_1_0;
			break;
		}
		case nbl::nEnumRenderFeatureLevel::PC:
		{
			AppInfo.apiVersion = VK_API_VERSION_1_3;
			break;
		}
		case nbl::nEnumRenderFeatureLevel::Mobile:
		{
			AppInfo.apiVersion = VK_API_VERSION_1_0;
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

		if (Info.bEnableValidationLayer)
		{
			VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
			InstanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(InstanceLayers.size());
			InstanceCreateInfo.ppEnabledLayerNames = InstanceLayers.data();

			PopulateDebugMessengerCreateInfo(debugCreateInfo);
			InstanceCreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
		}
	}

	VkResult Result = vkCreateInstance(&InstanceCreateInfo, nullptr, &Instance);

	/*
		创建实例成功后，开启校验层
	*/
	if (Result == VK_SUCCESS)
	{
		if (Info.bEnableValidationLayer)
		{
			VkDebugUtilsMessengerCreateInfoEXT createInfo;
			PopulateDebugMessengerCreateInfo(createInfo);

			Result = CreateDebugUtilsMessengerEXT(Instance, &createInfo, nullptr, &DebugMessenger);
		}
	}

	return Result;
}

void nbl::nVulkanRHI::DestroyInstance()
{
	/*
		卸载校验层
	*/
	if (Info.bEnableValidationLayer)
	{
		PFN_vkDestroyDebugUtilsMessengerEXT DestroyDebugUtilsMessengerFunc = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(Instance, "vkDestroyDebugUtilsMessengerEXT");
		if (DestroyDebugUtilsMessengerFunc != nullptr) 
			DestroyDebugUtilsMessengerFunc(Instance, DebugMessenger, nullptr);
	}

	if (Instance != VK_NULL_HANDLE)
		vkDestroyInstance(Instance, nullptr);
}

void nbl::nVulkanRHI::PopulatePhysicalDevice()
{
}
