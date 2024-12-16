#pragma once
#include "NewPluginsModuleDefines.h"
#include <Module/ModuleManager.h>

namespace nbl
{
	class NEWPLUGINS_API nNewPluginsModule: public nIModule
	{
		GENERATED_MODULE(nNewPluginsModule)

	protected:
		nNewPluginsModule() {}
	public:
		~nNewPluginsModule() {}

	};
}
