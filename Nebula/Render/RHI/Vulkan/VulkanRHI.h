#pragma once
/*
	��ʼ��Vulkan�Ĳ��裺
	��������Ҫ�õ�Layer
	��������Ҫ�õ���Vulkan��չ����
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
			ʵ����Layer��Ext + �豸��Ext
		*/
		std::vector<const char*> InstanceLayers;
		std::vector<const char*> InstanceEXTs;
		std::vector<const char*> DeviceEXTs;

		/*
			�Ƿ���Ҫpresent���ǲ��Ǵ���Ӧ�ó����й�
		*/
		bool bNeedPresent = false;

		/*
			�Ƿ���Ҫͼ�κͼ��㹦�ܺ�FeatureLevel�й�
		*/
		bool bNeedGraphic = false;
		bool bNeedCompute = false;

		/*
			��������ֶ�����
		*/
		void PopulateRequirements();

	private:
		// ������
		VkInstance Instance = VK_NULL_HANDLE;

		// ��ѡ��Ƿ���ҪDebug��
		VkDebugUtilsMessengerEXT DebugMessenger;

		// ��ѡ��Ƿ��Ǵ���Ӧ�ó���
		VkSurfaceKHR Surface = VK_NULL_HANDLE;

		/*
			��������������Դ
		*/
		VkResult CreateInsatnce();
		void DestroyInstance();
	private:

		// ��ѯ֧��Ӧ�ó�������Ҫ��������豸
		std::vector<VkPhysicalDevice> SuitablePhysicalDevices;
		VkPhysicalDevice PhysicalDevice = VK_NULL_HANDLE;

		/*
			��������ֶ�
		*/
		void PopulatePhysicalDevice();

	private:

#pragma region �߼�GPU
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

		VkExtent2D SwapChainExtent;/*����ʱ�ı�*/

		// ���ڽ�������n���壩
		VkSwapchainKHR SwapChain = VK_NULL_HANDLE;

		// n�����Ӧ��n��vk image
		std::vector<VkImage> SwapChainImages;
		std::vector<VkImageView> SwapChainImageViews;

		void DestroySwapChain();
	};
}