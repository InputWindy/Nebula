#include "OpenGLRHI.h"

#include <glad/glad.h>
#include <cassert>
#include <exception>
#include <stdexcept>

#include <PlatformCore.h>

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
		NewInfo->PlatformWindow&&
		NewInfo->PlatformWindow->IsValid()
		)
	{
		return gladLoadGLLoader((GLADloadproc)Info.PlatformWindow) ? nEnumRHIInitResult::Success : nEnumRHIInitResult::InvalidGetProcCallback;
	}
	else
	{
		return nEnumRHIInitResult::InvalidPlatformWindow;
	}
}