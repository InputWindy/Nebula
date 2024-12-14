#pragma once
#include "RenderModuleDefines.h"
#include <Module/ModuleManager.h>

namespace nbl
{
	class RENDER_API nRenderModule: public nIModule
	{
	public:
		nRenderModule() {}
		~nRenderModule() {}
	};

	extern template nRenderModule& nModuleManager::LoadModule<nRenderModule>();
}
