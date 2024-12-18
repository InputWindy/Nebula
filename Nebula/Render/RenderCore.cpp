#include "RenderCore.h"
#include <iostream>

#include "RHI/OpenGL/OpenGLRHI.h"
#include "RHI/Vulkan/VulkanRHI.h"
#include "Window/Glfw/GlfwWindow.h"
#include "Window/Egl/EglWindow.h"
#include <GLFW/glfw3.h>

#include <cassert>

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
	nRHIAccessor nRenderModule::GetRHIChecked() const
	{
		if (!IsValidRHI())
		{
			throw std::runtime_error("invalid rhi .");
		}

		return nRHIAccessor(GetRHI());
	}
	nRHIAccessor nRenderModule::GetRHI() const
	{
		return nRHIAccessor(RenderInterface.get());
	}

	bool nRenderModule::CreatePlatformWindow(const nPlatformWindowCreateInfo& Info)
	{
		switch (Info.BackendType)
		{
		case nbl::nEnumWindowBackend::Glfw:
		{
			PlatformWindow = std::make_unique<nGlfwWindow>(Info);
			break;
		}
		case nbl::nEnumWindowBackend::Egl:
		{
			PlatformWindow = std::make_unique<nEglWindow>(Info);
			break;
		}
		default:
			break;
		}

		return IsValidPlatformWindow();
	}
	bool nRenderModule::IsValidPlatformWindow() const
	{
		return PlatformWindow && PlatformWindow->IsValid();
	}
	nPlatformWindowAccessor nRenderModule::GetPlatformWindowChecked()
	{
		bool bValid = IsValidPlatformWindow();

		assert(bValid, "invalid platform window .");

		if (!bValid)
			throw std::runtime_error("invalid platform window .");

		return GetPlatformWindow();
	}
	nPlatformWindowAccessor nRenderModule::GetPlatformWindow()
	{
		return nPlatformWindowAccessor(PlatformWindow.get());
	}
}
