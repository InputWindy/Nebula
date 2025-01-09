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

		void PopulateRequirements();

	private:
		VkInstance Instance = VK_NULL_HANDLE;

		// ����ǿ�ѡ��
		VkDebugUtilsMessengerEXT DebugMessenger;

		VkResult CreateInsatnce();
		void DestroyInstance();
	private:

		// ����Ӧ�ó�������Ҫ��������豸
		std::vector<VkPhysicalDevice> SuitablePhysicalDevices;
		VkPhysicalDevice PhysicalDevice = VK_NULL_HANDLE;

		void PopulatePhysicalDevice();

	private:


#pragma region �߼�GPU
		VkDevice LogicDevice = VK_NULL_HANDLE;

		VkQueue GraphicsQueue = VK_NULL_HANDLE;
		VkQueue ComputeQueue = VK_NULL_HANDLE;
#pragma endregion

		VkResult CreateLogicDevice();
		void DestroyLogicDevice();

	private:


#pragma region ����Ӧ�ó������

		// ������ʾ�������
		VkQueue PresentQueue;

		// ���ڳ���
		VkSurfaceKHR Surface;

		// ���ڽ�������n���壩
		VkSwapchainKHR SwapChain;

		// n�����Ӧ��n��vk image
		std::vector<VkImage> SwapChainImages;

		// n����image��Ӧ�Ĳ�����������ں�framebuffer��������
		std::vector<VkImageView> SwapChainImageViews;

		// ������ʾ
		VkFormat SwapChainImageFormat;
		VkExtent2D SwapChainExtent;

#pragma endregion

	};
}