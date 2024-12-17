#include "VulkanRHI.h"
#include <vulkan/vulkan.h>

#include <stdexcept>

#include <PlatformCore.h>

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

extern "C" const char** glfwGetRequiredInstanceExtensions(uint32_t*);

nbl::nEnumRHIInitResult nbl::nVulkanRHI::InitBackend(nbl::nRHICreateInfo* NewInfo)
{
	nEnumRHIInitResult Result = nRHI::InitBackend(NewInfo);
	if(Result == nEnumRHIInitResult::EmptyCreateInfo)return Result;

	// check layer available
	if (Info.bEnableValidationLayers && !IsSupportedLayer(Info.ValidationLayers))
		return nEnumRHIInitResult::InavailableLayers;

	if (
		!Info.PlatformWindow ||
		!Info.PlatformWindow->IsValid() ||
		Info.PlatformWindow->GetType() != nEnumWindowBackend::Glfw
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
	auto extensions = GetRequiredExtensions(Info);
	CreateInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	CreateInfo.ppEnabledExtensionNames = extensions.data();

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

bool nbl::nVulkanRHI::IsSupportedLayer(const std::vector<std::string>& ValidationLayers) const
{
	for (const std::string& LayerName : ValidationLayers) 
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
