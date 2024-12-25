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

	bool nRenderModule::Init(const nRenderModuleCreateInfo& NewInfo)
	{
		CreateInfo = NewInfo;

		nPlatformWindowCreateInfo WindowInfo;
		WindowInfo.Title = NewInfo.Title;
		WindowInfo.W = NewInfo.W;
		WindowInfo.H = NewInfo.H;
		WindowInfo.bHideWindow = NewInfo.SurfaceType == nEnumSurfaceType::OffScreen;
		WindowInfo.bResizable = NewInfo.bResizable;
		WindowInfo.UserData = NewInfo.UserData;

#if defined(NBL_VK_ENABLED)
		WindowInfo.RenderBackend = nEnumRenderBackend::Vulkan;
#else
		WindowInfo.RenderBackend = nEnumRenderBackend::OpenGL;
#endif

#if defined(_WIN32)
		PlatformWindow = std::make_unique<nGlfwWindow>(WindowInfo);
#elif defined(__linux__)
		PlatformWindow = std::make_unique<nEglWindow>(WindowInfo);
#elif defined(__APPLE__)
#else
#endif

		nRHICreateInfo RHIInfo(GetPlatformWindow());

		switch (RHIInfo.PlatformWindow.GetInfo().RenderBackend)
		{
		case nbl::nEnumRenderBackend::Vulkan:
		{
			nVulkanRHI* RHI = new nVulkanRHI();
			assert( RHI->InitBackend(&RHIInfo) == nEnumRHIInitResult::Success);

			RenderInterface.reset(RHI);
			
			break;
		}
		case nbl::nEnumRenderBackend::OpenGL:
		{
			nOpenGLRHI* RHI = new nOpenGLRHI();
			assert(RHI->InitBackend(&RHIInfo) == nEnumRHIInitResult::Success);

			RenderInterface.reset(RHI);
			break;
		}
		default:
			break;
		}

		return IsValidPlatformWindow() && IsValidRHI();
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
