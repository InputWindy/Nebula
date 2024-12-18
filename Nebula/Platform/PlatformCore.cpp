#include "PlatformCore.h"
#include <iostream>

#include "Window/Glfw/GlfwWindow.h"
#include "Window/Egl/EglWindow.h"

#include <assert.h>

namespace nbl
{
	IMPL_MODULE(PLATFORM_API,nPlatformModule)
	
	bool nPlatformModule::StartupModule()
	{
		std::cout << __FUNCTION__ << std::endl;
		return true;
	}
	bool nPlatformModule::ShutdownModule()
	{
		std::cout << __FUNCTION__ << std::endl;
		return true;
	}



	bool nPlatformModule::CreatePlatformWindow(const nPlatformWindowCreateInfo& Info)
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
	bool nPlatformModule::IsValidPlatformWindow() const
	{
		return PlatformWindow && PlatformWindow->IsValid();
	}
	nPlatformWindowAccessor nPlatformModule::GetPlatformWindowChecked()
	{
		bool bValid = IsValidPlatformWindow();

		assert(bValid, "invalid platform window .");

		if (!bValid)
			throw std::runtime_error("invalid platform window .");

		return GetPlatformWindow();
	}
	nPlatformWindowAccessor nPlatformModule::GetPlatformWindow()
	{
		return nPlatformWindowAccessor(PlatformWindow.get());
	}
}


