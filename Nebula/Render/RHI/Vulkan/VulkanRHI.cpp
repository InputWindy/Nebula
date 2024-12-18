#include "VulkanRHI.h"
#include <vulkan/vulkan.h>

#include <stdexcept>

#include <PlatformCore.h>
#include <iostream>

#include "RHI/RHI.h"

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
		vk是非常底层的协议，很多额外功能都是以拓展形式实现的，这里就是配置的是vulkan的窗口拓展（VK_KHR_swapchain）。
		类似的拓展还有VK_KHR_ray_tracing 支持光追、VK_KHR_multiview 支持VR/AR
	*/
	auto Extensions = GetRequiredExtensions(Info);
	CreateInfo.enabledExtensionCount = static_cast<uint32_t>(Extensions.size());
	CreateInfo.ppEnabledExtensionNames = Extensions.data();

	//开启Layer。Layer相当于在Vulkan代码里嵌入代码，主要是给RenderDoc这类代码分析工具用的。
	if (Info.bEnableValidationLayers) 
	{
		VkDebugUtilsMessengerCreateInfoEXT DebugCreateInfo{};

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

	if (vkCreateInstance(&CreateInfo, nullptr, &Instance) != VK_SUCCESS) 
	{
		throw std::runtime_error("failed to create instance!");
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
			if (LayerName == LayerProperties.layerName) 
			{
				bLayerFound = true;
				break;
			}
		}

		if (!bLayerFound)
			return false;
		
	}

	return false;
}
