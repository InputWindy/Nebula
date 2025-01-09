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
		/*
			实例级Layer、Ext + 设备级Ext
		*/
		std::vector<const char*> InstanceLayers;
		std::vector<const char*> InstanceEXTs;
		std::vector<const char*> DeviceEXTs;

		/*
			是否需要present和是不是窗口应用程序有关
		*/
		bool bNeedPresent = false;

		/*
			是否需要图形和计算功能和FeatureLevel有关
		*/
		bool bNeedGraphic = false;
		bool bNeedCompute = false;

		void PopulateRequirements();

	private:
		VkInstance Instance = VK_NULL_HANDLE;

		// 这个是可选项
		VkDebugUtilsMessengerEXT DebugMessenger;

		VkResult CreateInsatnce();
		void DestroyInstance();
	private:

		// 符合应用程序特性要求的物理设备
		std::vector<VkPhysicalDevice> SuitablePhysicalDevices;
		VkPhysicalDevice PhysicalDevice = VK_NULL_HANDLE;

		void PopulatePhysicalDevice();

	private:


#pragma region 逻辑GPU
		VkDevice LogicDevice = VK_NULL_HANDLE;

		VkQueue GraphicsQueue = VK_NULL_HANDLE;
		VkQueue ComputeQueue = VK_NULL_HANDLE;
#pragma endregion

		VkResult CreateLogicDevice();
		void DestroyLogicDevice();

	private:


#pragma region 窗口应用程序相关

		// 窗口显示命令队列
		VkQueue PresentQueue;

		// 窗口抽象
		VkSurfaceKHR Surface;

		// 窗口交换链（n缓冲）
		VkSwapchainKHR SwapChain;

		// n缓冲对应的n张vk image
		std::vector<VkImage> SwapChainImages;

		// n缓冲image对应的操作句柄（用于和framebuffer做交互）
		std::vector<VkImageView> SwapChainImageViews;

		// 窗口显示
		VkFormat SwapChainImageFormat;
		VkExtent2D SwapChainExtent;

#pragma endregion

	};
}