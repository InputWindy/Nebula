/*
	模块内头文件引用使用""
	模块外头文件引用使用<>

	以防出现跨模块同名文件引用错误导致报错
*/
#pragma once
#include "PlatformModuleDefines.h"

#include "Module/ModuleManager.h"
#include "Window/PlatformWindow.h"

namespace nbl
{
	enum nPlatformErrorCode:unsigned int
	{
		
	};

	class PLATFORM_API nPlatformModule final: public nIModule
	{
		GENERATED_MODULE(nPlatformModule)

	public:
		/// <summary>
		/// create native window
		/// </summary>
		/// <param name="NewInfo"></param>
		/// <returns>valid</returns>
		bool CreatePlatformWindow(const nPlatformWindowCreateInfo&, nEnumWindowBackend);

		/// <summary>
		/// check validation
		/// </summary>
		/// <returns></returns>
		bool IsValidPlatformWindow()const;

		/// <summary>
		/// get valid window
		/// </summary>
		/// <returns>p window</returns>
		nPlatformWindow* GetPlatformWindowChecked();

		/// <summary>
		/// get raw ptr
		/// </summary>
		/// <returns>p window(can be null)</returns>
		nPlatformWindow* GetPlatformWindow();

	private:
		std::unique_ptr<nPlatformWindow> PlatformWindow;
	};
}
