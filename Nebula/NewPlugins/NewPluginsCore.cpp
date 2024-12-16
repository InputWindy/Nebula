#include "NewPluginsCore.h"
#include <iostream>

namespace nbl
{
	IMPL_MODULE(NEWPLUGINS_API,nNewPluginsModule)
	
	bool nNewPluginsModule::StartupModule()
	{
		std::cout << __FUNCTION__ << std::endl;
		return true;
	}
	bool nNewPluginsModule::ShutdownModule()
	{
		std::cout << __FUNCTION__ << std::endl;
		return true;
	}
}


