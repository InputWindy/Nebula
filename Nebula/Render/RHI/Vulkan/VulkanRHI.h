#pragma once
#include "RHI/RHI.h"

#include <vulkan/vulkan.h>

#include <vector>
#include <string>

namespace nbl
{
	struct nVulkanCreateInfo
	{
		/*
			Ð£Ñé²ã
		*/
		std::vector<std::string> ValidationLayers = { "VK_LAYER_KHRONOS_validation" };

#ifdef NDEBUG
		const bool bEnableValidationLayers = false;
#else
		const bool bEnableValidationLayers = true;
#endif // NDEBUG

	};

	class RENDER_API nVulkanRHI:public nRHI
	{
	public:
		nVulkanRHI();
		virtual ~nVulkanRHI();

		virtual nEnumRenderBackend GetType()const override final;

		virtual bool InitBackend(void*)const override final;
	private:
		std::vector<VkLayerProperties> AvailableLayers;
		nVulkanCreateInfo Info;
	};
}