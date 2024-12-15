#include "RenderCore.h"
#include <iostream>

namespace nbl
{
	IMPL_MODULE(RENDER_API,nRenderModule)
	
	bool nRenderModule::StartupModule()
	{
		std::cout << __FUNCTION__ << std::endl;
		return true;
	}
	bool nRenderModule::ShutdownModule()
	{
		std::cout << __FUNCTION__ << std::endl;
		return true;
	}
}


