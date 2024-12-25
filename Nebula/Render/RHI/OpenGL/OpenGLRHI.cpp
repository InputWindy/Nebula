#include "OpenGLRHI.h"

#include <glad/glad.h>
#include <cassert>
#include <exception>
#include <stdexcept>

nbl::nOpenGLRHI::nOpenGLRHI()
{
}

nbl::nOpenGLRHI::~nOpenGLRHI()
{
}

nbl::nEnumRenderBackend nbl::nOpenGLRHI::GetType() const
{
	return nEnumRenderBackend::OpenGL;
}

nbl::nEnumRHIInitResult nbl::nOpenGLRHI::InitBackend(nRHICreateInfo* NewInfo)
{
	if (
		nRHI::InitBackend(NewInfo) == nEnumRHIInitResult::Success &&
		NewInfo->PlatformWindow.IsValid()
		)
	{
		auto Result = gladLoadGLLoader((GLADloadproc)Info.PlatformWindow.GetProcAddressCallbackFunc()) ? nEnumRHIInitResult::Success : nEnumRHIInitResult::InvalidGetProcCallback;

		return Result;
	}
	else
	{
		return nEnumRHIInitResult::InvalidPlatformWindow;
	}
}
