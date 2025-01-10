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
#include <optional>

namespace nbl
{

	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	typedef VKAPI_ATTR VkBool32(*VkDebugCallback)(VkDebugUtilsMessageSeverityFlagBitsEXT, VkDebugUtilsMessageTypeFlagsEXT, const VkDebugUtilsMessengerCallbackDataEXT*, void*);

	class RENDER_API nVulkanRHI:public nRHI
	{
	public:
		nVulkanRHI();
		virtual ~nVulkanRHI();

		virtual nEnumRenderBackend GetType()const override final;

		virtual nEnumRHIInitResult InitBackend(nRHICreateInfo*) override final;
	
	public:

		virtual bool IsDisplaySupported()const override;
		virtual bool IsGraphicSupported()const override;
		virtual bool IsComputeSupported()const override;
		virtual bool IsValid()const override;


	public:
		VkResult RegenerateSwapChain();
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

		/*
			填充以上字段内容
		*/
		void PopulateRequirements();

	private:
		// 必须项
		VkInstance Instance = VK_NULL_HANDLE;

		// 可选项（是否需要Debug）
		VkDebugUtilsMessengerEXT DebugMessenger;

		// 可选项（是否是窗口应用程序）
		VkSurfaceKHR Surface = VK_NULL_HANDLE;

		/*
			创建销毁上述资源
		*/
		VkResult CreateInsatnce();
		void DestroyInstance();
	private:

		// 查询支持应用程序特性要求的物理设备
		std::vector<VkPhysicalDevice> SuitablePhysicalDevices;
		VkPhysicalDevice PhysicalDevice = VK_NULL_HANDLE;

		/*
			填充上述字段
		*/
		void PopulatePhysicalDevice();

	private:

#pragma region 逻辑GPU
		VkDevice LogicDevice = VK_NULL_HANDLE;

		VkQueue GraphicsQueue = VK_NULL_HANDLE;
		VkQueue ComputeQueue = VK_NULL_HANDLE;
		VkQueue PresentQueue = VK_NULL_HANDLE;

		std::optional<uint32_t> computeFamily;
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

#pragma endregion

		VkResult CreateLogicDevice();
		void DestroyLogicDevice();

	private:

		VkExtent2D SwapChainExtent;/*运行时改变*/

		// 窗口交换链（n缓冲）
		VkSwapchainKHR SwapChain = VK_NULL_HANDLE;

		// n缓冲对应的n张vk image
		std::vector<VkImage> SwapChainImages;
		std::vector<VkImageView> SwapChainImageViews;

		void DestroySwapChain();
	};
}