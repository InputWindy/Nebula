#pragma once
#include "PlatformModuleDefines.h"

#include <string>
#include <Module/ModuleManager.h>

namespace nbl
{
	enum class nEnumWindowBackend
	{
		Glfw,
		Egl
	};

	struct PLATFORM_API nPlatformWindowCreateInfo
	{
		std::string Title;

		int W = 800;
		int H = 600;

		void* UserData = nullptr;
		bool  bHideWindow = false;
		bool  bResizable  = true;
		bool  bOpenGLBackend = false;

		nEnumWindowBackend BackendType;
	};

	class PLATFORM_API nPlatformWindow
	{
	protected:
		nPlatformWindow();
	public:
		virtual ~nPlatformWindow();

		virtual void* GetHandle()  const = 0;
		virtual void* GetUserData()const = 0;
		virtual int   GetW()	   const = 0;
		virtual int   GetH()	   const = 0;
		virtual bool  IsValid()    const = 0;
		 
		virtual std::string			GetWindowTitle()	const = 0;
		virtual nEnumWindowBackend  GetType()			const = 0;

		virtual void* GetProcAddressCallbackFunc()const = 0;

	public:
		virtual bool ShouldClose()const = 0;
		virtual void PollEvent()  const = 0;
	};

	struct PLATFORM_API nPlatformWindowAccessor final:public nModulePtrAccessor<nPlatformWindow>
	{
		GENERATED_PTR_ACCESSOR(nPlatformWindowAccessor)
	public:

#ifdef PLATFORM_MODULE
		void* GetHandle()  const;
#endif

		void* GetUserData()const;
		int   GetW()	   const;
		int   GetH()	   const;
		bool  IsValid()    const;

		std::string			GetWindowTitle()	const;
		nEnumWindowBackend  GetType()			const;

		void* GetProcAddressCallbackFunc()const;

	public:
		bool ShouldClose()const;
		void PollEvent()  const;
	};
}
