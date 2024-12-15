#pragma once
#include "RenderModuleDefines.h"
#include <Module/ModuleManager.h>

namespace nbl
{
	class RENDER_API nRenderModule: public nIModule
	{
		GENERATED_MODULE(nRenderModule)

		friend class nModuleManager;
	protected:
		nRenderModule() {}
	public:
		~nRenderModule() {}

	};
}
