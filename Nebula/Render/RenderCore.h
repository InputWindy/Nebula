#pragma once
#include "RenderModuleDefines.h"
#include <Module/ModuleManager.h>

namespace nbl
{
	class RENDER_API nRenderModule: public nIModule
	{
		friend class nModuleManager;
	
	protected:
		nRenderModule() {}
	public:
		~nRenderModule() {}

		void UnitTest();

	};

	extern template RENDER_API nRenderModule& nModuleManager::LoadModule<nRenderModule>();

}
