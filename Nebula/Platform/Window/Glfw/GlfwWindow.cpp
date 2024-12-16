#include "GlfwWindow.h"

#ifdef ENABLE_WINDOW
	#include <GLFW/glfw3.h>
#endif

nbl::nGlfwWindow::nGlfwWindow(const nPlatformWindowCreateInfo& NewInfo) 
	:Info(NewInfo),Handle(nullptr)
{
#ifdef ENABLE_WINDOW
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, NewInfo.bResizable ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_VISIBLE, NewInfo.bHideWindow ? GLFW_FALSE : GLFW_TRUE);

	Handle = glfwCreateWindow(Info.W, Info.H, Info.Title.c_str(), nullptr, nullptr);
#elif
	Handle = nullptr;
#endif

}

nbl::nGlfwWindow::~nGlfwWindow()
{
#ifdef ENABLE_WINDOW
	glfwDestroyWindow(Handle);
	glfwTerminate();
#endif
	
}

void* nbl::nGlfwWindow::GetHandle() const
{
	return Handle;
}

bool nbl::nGlfwWindow::IsValid() const
{
	return Handle;
}

std::string nbl::nGlfwWindow::GetWindowTitle() const
{
	return Info.Title;
}

nbl::nEnumWindowBackend nbl::nGlfwWindow::GetType() const
{
	return nEnumWindowBackend::Glfw;
}

bool nbl::nGlfwWindow::ShouldClose() const
{
	return glfwWindowShouldClose(Handle);
}

void nbl::nGlfwWindow::PollEvent() const
{
#ifdef ENABLE_WINDOW
	glfwPollEvents();
#endif 

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
