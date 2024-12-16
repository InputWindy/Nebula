#pragma once
#include "PlatformModuleDefines.h"
#include <Module/ModuleManager.h>

#include "Window/PlatformWindow.h"

namespace nbl
{
	class PLATFORM_API nPlatformModule: public nIModule
	{
		GENERATED_MODULE(nPlatformModule)

	protected:
		nPlatformModule() {}
	public:
		virtual ~nPlatformModule() {}

		/// <summary>
		/// create native window
		/// </summary>
		/// <param name="NewInfo"></param>
		/// <returns>valid</returns>
		virtual bool  CreatePlatformWindow(const nPlatformWindowCreateInfo&, nEnumWindowBackend);

		/// <summary>
		/// check validation
		/// </summary>
		/// <returns></returns>
		virtual bool  IsValidPlatformWindow()const;

		/// <summary>
		/// get valid window
		/// </summary>
		/// <returns>p window</returns>
		virtual nPlatformWindow* GetPlatformWindowChecked();

		/// <summary>
		/// get raw ptr
		/// </summary>
		/// <returns>p window(can be null)</returns>
		virtual nPlatformWindow* GetPlatformWindow();

	private:
		std::unique_ptr<nPlatformWindow> PlatformWindow;
	};
}
