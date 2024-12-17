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
	bool nRenderModule::CreateRHI(nRHICreateInfo* NewInfo)
	{
		switch (NewInfo->BackendType)
		{
		case nbl::nEnumRenderBackend::Vulkan:
		{
			RenderInterface = std::make_unique<nVulkanRHI>();
			RenderInterface->InitBackend(NewInfo);
			break;
		}
		case nbl::nEnumRenderBackend::OpenGL:
		{
			RenderInterface = std::make_unique<nOpenGLRHI>();
			RenderInterface->InitBackend(NewInfo);
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
	bool nRenderModule::InitRHI(nRHICreateInfo* Ptr)
	{
		bool bRet = false;

		if (nRHI* RHI = GetRHIChecked())
		{
			switch (RHI->GetType())
			{
			case nbl::nEnumRenderBackend::Vulkan:
			{
				bRet = RHI->InitBackend(Ptr) == nEnumRHIInitResult::Success;
				break;
			}
			case nbl::nEnumRenderBackend::OpenGL:
			{
				bRet = RHI->InitBackend(Ptr) == nEnumRHIInitResult::Success;
				break;
			}
			default:
				break;
			}
		}

		return bRet;
	}
}


