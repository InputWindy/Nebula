#pragma once
#include "../PlatformWindow.h"

class GLFWwindow;

namespace nbl
{
	class RENDER_API nGlfwWindow:public nPlatformWindow
	{
	public:
		nGlfwWindow(const nPlatformWindowCreateInfo& NewInfo);
		virtual ~nGlfwWindow();
		virtual void* GetHandle()  const override final;
		virtual void* GetUserData()const override final;
		virtual int   GetW()	   const override final;
		virtual int   GetH()	   const override final;
		virtual bool  IsValid()    const override final;

		virtual void* GetProcAddressCallbackFunc()const override final;

		virtual const nPlatformWindowCreateInfo& GetInfo() const override final;

		virtual bool ShouldClose()const override final;
		virtual void PollEvent()  const override final;
	private:
		const nPlatformWindowCreateInfo& Info;
		GLFWwindow* Handle = nullptr;
	};

}
