#include "PlatformWindow.h"
#include <GLFW/glfw3.h>

nbl::nPlatformWindow::nPlatformWindow()
{
}

nbl::nPlatformWindow::~nPlatformWindow()
{
}

void* nbl::nPlatformWindowAccessor::GetHandle() const
{
    return Ptr ? Ptr->GetHandle() : Ptr;
}


void* nbl::nPlatformWindowAccessor::GetUserData() const
{
    return Ptr ? Ptr->GetUserData() : Ptr;
}

int nbl::nPlatformWindowAccessor::GetW() const
{
    return Ptr? Ptr->GetW():0;
}

int nbl::nPlatformWindowAccessor::GetH() const
{
    return Ptr? Ptr->GetH():0;
}

bool nbl::nPlatformWindowAccessor::IsValid() const
{
    return Ptr&& Ptr->IsValid();
}


const nbl::nPlatformWindowCreateInfo& nbl::nPlatformWindowAccessor::GetInfo() const
{
    static nPlatformWindowCreateInfo msInfo;
    return Ptr ? Ptr->GetInfo() : msInfo;
}

void* nbl::nPlatformWindowAccessor::GetProcAddressCallbackFunc() const
{
    return Ptr?Ptr->GetProcAddressCallbackFunc(): Ptr;
}

bool nbl::nPlatformWindowAccessor::ShouldClose() const
{
    return Ptr && Ptr->ShouldClose();
}

void nbl::nPlatformWindowAccessor::PollEvent() const
{
    if(Ptr)Ptr->PollEvent();
}
