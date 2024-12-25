#include "GlfwWindow.h"
#include "RHI/RHI.h"


#define GLFW_INCLUDE_VULKAN
#include<GLFW/glfw3.h>

nbl::nGlfwWindow::nGlfwWindow(const nPlatformWindowCreateInfo& NewInfo) 
	:Info(NewInfo),Handle(nullptr)
{
	glfwInit();
	glfwDefaultWindowHints();
	 
	glfwWindowHint(GLFW_RESIZABLE, NewInfo.bResizable ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_VISIBLE, NewInfo.bHideWindow ? GLFW_FALSE : GLFW_TRUE);

	if (NewInfo.RenderBackend != nEnumRenderBackend::OpenGL)
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	Handle = glfwCreateWindow(Info.W, Info.H, Info.Title.c_str(), nullptr, nullptr);
	glfwMakeContextCurrent((GLFWwindow*)Handle);
}

nbl::nGlfwWindow::~nGlfwWindow()
{
	glfwDestroyWindow(Handle);
	glfwTerminate();
}

void* nbl::nGlfwWindow::GetHandle() const
{
	return Handle;
}

bool nbl::nGlfwWindow::IsValid() const
{
	return Handle;
}

void* nbl::nGlfwWindow::GetProcAddressCallbackFunc() const
{
	return (void*)glfwGetProcAddress;
}

const nbl::nPlatformWindowCreateInfo& nbl::nGlfwWindow::GetInfo() const
{
	return Info;
}

bool nbl::nGlfwWindow::ShouldClose() const
{
	return glfwWindowShouldClose(Handle);
}

void nbl::nGlfwWindow::PollEvent() const
{
	glfwPollEvents();
}

void* nbl::nGlfwWindow::GetUserData() const
{
	return Info.UserData;
}

int nbl::nGlfwWindow::GetW() const
{
	return Info.W;
}

int nbl::nGlfwWindow::GetH() const
{
	return Info.H;
}
