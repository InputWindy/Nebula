#pragma once
#include "RHI/RHI.h"

#include <vulkan/vulkan.h>

#include <vector>
#include <string>

namespace nbl
{

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

		bool IsSupportedLayer(const std::vector<std::string>&)const;
	private:
		std::vector<VkLayerProperties> AvailableLayers;
	};
}