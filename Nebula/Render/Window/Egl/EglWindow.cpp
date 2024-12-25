#include "EglWindow.h"

nbl::nEglWindow::nEglWindow(const nPlatformWindowCreateInfo& NewInfo)
    :Info(NewInfo)
{

}

nbl::nEglWindow::~nEglWindow()
{
}

void* nbl::nEglWindow::GetHandle() const
{
    return nullptr;
}

void* nbl::nEglWindow::GetUserData() const
{
    return nullptr;
}

int nbl::nEglWindow::GetW() const
{
    return 0;
}

int nbl::nEglWindow::GetH() const
{
    return 0;
}

bool nbl::nEglWindow::IsValid() const
{
    return false;
}

void* nbl::nEglWindow::GetProcAddressCallbackFunc() const
{
    return nullptr;
}

const nbl::nPlatformWindowCreateInfo& nbl::nEglWindow::GetInfo() const
{
    return Info;
}

bool nbl::nEglWindow::ShouldClose() const
{
    return false;
}

void nbl::nEglWindow::PollEvent() const
{
}
