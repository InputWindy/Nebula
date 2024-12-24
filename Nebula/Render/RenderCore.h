/*
	模块内头文件引用使用""
	模块外头文件引用使用<>

	以防出现跨模块同名文件引用错误导致报错
*/
#pragma once
#include "RenderModuleDefines.h"
#include <Module/ModuleManager.h>

#include "RHI/RHI.h"
#include "Window/PlatformWindow.h"
#include "RHI/RHI.h"

namespace nbl
{
	class RENDER_API nRenderModule final: public nIModule
	{
		GENERATED_MODULE(nRenderModule)
	public:
		/// <summary>
		/// create native window
		/// </summary>
		/// <param name="NewInfo"></param>
		/// <returns>valid</returns>
		bool CreatePlatformWindow(const nPlatformWindowCreateInfo&);

		/// <summary>
		/// check validation
		/// </summary>
		/// <returns></returns>
		bool IsValidPlatformWindow()const;

		/// <summary>
		/// get valid window
		/// </summary>
		/// <returns>p window</returns>
		nPlatformWindowAccessor GetPlatformWindowChecked();

		/// <summary>
		/// get raw ptr
		/// </summary>
		/// <returns>p window(can be null)</returns>
		nPlatformWindowAccessor GetPlatformWindow();

	private:
		std::unique_ptr<nPlatformWindow> PlatformWindow;
	public:

		bool  CreateRHI(nRHICreateInfo*);
		bool  IsValidRHI()const;
		nRHIAccessor GetRHIChecked()const;
		nRHIAccessor GetRHI()const;

	private:
		std::unique_ptr<nRHI> RenderInterface;
	};
}
