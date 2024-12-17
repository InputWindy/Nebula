#include "VulkanRHI.h"
#include <vulkan/vulkan.h>

nbl::nVulkanRHI::nVulkanRHI()
{
}

nbl::nVulkanRHI::~nVulkanRHI()
{
}

nbl::nEnumRenderBackend nbl::nVulkanRHI::GetType() const
{
	return nEnumRenderBackend::Vulkan;
}

bool nbl::nVulkanRHI::InitBackend(void*) const
{
	return false;
}
