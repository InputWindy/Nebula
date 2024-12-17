/*
	模块内头文件引用使用""
	模块外头文件引用使用<>

	以防出现跨模块同名文件引用错误导致报错
*/
#pragma once
#include "RenderModuleDefines.h"
#include <Module/ModuleManager.h>

#include "RHI/RHI.h"

namespace nbl
{
	class RENDER_API nRenderModule final: public nIModule
	{
		GENERATED_MODULE(nRenderModule)

	public:

		bool  CreateRHI(nRHICreateInfo*);
		bool  IsValidRHI()const;
		nRHI* GetRHIChecked()const;
		nRHI* GetRHI()const;

	private:
		bool InitRHI(nRHICreateInfo*);

	private:
		std::unique_ptr<nRHI> RenderInterface;
	};
}
