#include "RenderCore.h"
#include <iostream>

#include "RHI/OpenGL/OpenGLRHI.h"
#include "RHI/Vulkan/VulkanRHI.h"

namespace nbl
{
	IMPL_MODULE(RENDER_API,nRenderModule)
	
	bool nRenderModule::StartupModule()
	{
		std::cout << __FUNCTION__ << std::endl;
		return true;
	}
	bool nRenderModule::ShutdownModule()
	{
		std::cout << __FUNCTION__ << std::endl;
		return true;
	}
	bool nRenderModule::CreateRHI(nEnumRenderBackend Type)
	{
		switch (Type)
		{
		case nbl::nEnumRenderBackend::Vulkan:
		{
			RenderInterface = std::make_unique<nVulkanRHI>();
			break;
		}
		case nbl::nEnumRenderBackend::OpenGL:
		{
			RenderInterface = std::make_unique<nOpenGLRHI>();
			break;
		}
		default:
			break;
		}
		return IsValidRHI();
	}
	bool nRenderModule::IsValidRHI() const
	{
		return RenderInterface.get();
	}
	nRHI* nRenderModule::GetRHIChecked() const
	{
		if (!IsValidRHI())
		{
			throw std::runtime_error("invalid rhi .");
		}

		return GetRHI();
	}
	nRHI* nRenderModule::GetRHI() const
	{
		return RenderInterface.get();
	}
	bool nRenderModule::InitRHI(void* Callback)
	{
		return false;
	}
}


