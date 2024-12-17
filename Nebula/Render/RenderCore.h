/*
	ģ����ͷ�ļ�����ʹ��""
	ģ����ͷ�ļ�����ʹ��<>

	�Է����ֿ�ģ��ͬ���ļ����ô����±���
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
