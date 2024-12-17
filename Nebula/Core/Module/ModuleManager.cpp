#include "ModuleManager.h"
#include <iostream>

namespace nbl
{



	

	IMPL_MODULE(CORE_API, nIModule)

	bool nIModule::StartupModule()
	{
		std::cout << __FUNCTION__ << std::endl;
		return true;
	}
	bool nIModule::ShutdownModule()
	{
		std::cout << __FUNCTION__ << std::endl;
		return true;
	}
}

namespace nbl
{
	IMPL_SINGLETON(nModuleManager)
	nResult nModuleManager::ShutdownModules()
	{
		nResult Result = Success;

		for (auto& Item : LoadedModules)
		{
			if(!Item.second->ShutdownModule())
				Result = Fail;
		}
		return Result;
	}
}