#pragma once
/*
	初始化Vulkan的步骤：
	①配置需要用的Layer
	②配置需要用到的Vulkan拓展特性
*/
#include "RHI/RHI.h"

#include <vulkan/vulkan.h>

#include <vector>
#include <string>

namespace nbl
{
	typedef VKAPI_ATTR VkBool32(*VkDebugCallback)(VkDebugUtilsMessageSeverityFlagBitsEXT, VkDebugUtilsMessageTypeFlagsEXT, const VkDebugUtilsMessengerCallbackDataEXT*, void*);

	class RENDER_API nVulkanRHI:public nRHI
	{
	public:
		nVulkanRHI();
		virtual ~nVulkanRHI();

		virtual nEnumRenderBackend GetType()const override final;

		virtual nEnumRHIInitResult InitBackend(nRHICreateInfo*) override final;
	
	public:
		const auto& GetCreateInfo()			const;
		const auto& GetAvailableLayers()	const;

		bool IsSupportedLayer(const std::vector<const char*>&)const;
	private:
		std::vector<VkLayerProperties> AvailableLayers;
	private:
		VkInstance Instance;
		VkDebugUtilsMessengerEXT DebugMessenger;
		VkSurfaceKHR Surface;

		std::vector<VkPhysicalDevice> PhysicalDevices;

		std::vector<VkPhysicalDevice> SuitablePhysicalDevices;

		VkPhysicalDevice PhysicalDevice = VK_NULL_HANDLE;
		VkDevice LogicDevice;

		VkQueue GraphicsQueue;
		VkQueue PresentQueue;
	};
}