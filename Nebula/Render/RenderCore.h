#pragma once
#include "RenderModuleDefines.h"
#include <Module/ModuleManager.h>

namespace nbl
{
	class RENDER_API nRenderModule: public nIModule
	{
		GENERATED_MODULE(nRenderModule)

	protected:
		nRenderModule() {}
	public:
		~nRenderModule() {}

	};
}
