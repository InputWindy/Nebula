#include "RHI.h"

#include <GLFW/glfw3.h>

nbl::nRHI::nRHI()
{
}

nbl::nEnumRHIInitResult nbl::nRHI::InitBackend(nRHICreateInfo* NewInfo)
{
    if (!NewInfo)return nEnumRHIInitResult::EmptyCreateInfo;

    Info = *NewInfo;

    return NewInfo->PlatformWindow.IsValid() ? nbl::nEnumRHIInitResult::Success : nbl::nEnumRHIInitResult::InvalidPlatformWindow;
}

nbl::nRHI::~nRHI()
{
}

const auto& nbl::nRHI::GetCreateInfo() const
{
	return Info;
}

bool nbl::nRHI::IsDisplaySupported() const
{
    return false;
}

bool nbl::nRHI::IsGraphicSupported() const
{
	return false;
}

bool nbl::nRHI::IsComputeSupported() const
{
	return false;
}

bool nbl::nRHI::IsValid() const
{
	return false;
}

//#include <glad/glad.h>

void nbl::nRHIAccessor::ClearColor()
{
    //glClearColor(0, 0, 1, 1);
}
