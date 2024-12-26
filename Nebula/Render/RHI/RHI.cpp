#include "RHI.h"

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

//#include <glad/glad.h>

void nbl::nRHIAccessor::ClearColor()
{
    //glClearColor(0, 0, 1, 1);
}
