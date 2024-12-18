#pragma once
#include "../PlatformWindow.h"

//class GLFWwindow;

namespace nbl
{
	class RENDER_API nEglWindow :public nPlatformWindow
	{
	public:
		nEglWindow(const nPlatformWindowCreateInfo& NewInfo);
		virtual ~nEglWindow();
		virtual void* GetHandle()  const override final;
		virtual void* GetUserData()const override final;
		virtual int   GetW()	   const override final;
		virtual int   GetH()	   const override final;
		virtual bool  IsValid()    const override final;

		virtual std::string			GetWindowTitle()	const override final;
		virtual nEnumWindowBackend  GetType()			const override final;

		virtual void* GetProcAddressCallbackFunc()const override final;

		virtual bool ShouldClose()const override final;
		virtual void PollEvent()  const override final;
	private:
		const nPlatformWindowCreateInfo& Info;
		void* Handle = nullptr;
	};

}
