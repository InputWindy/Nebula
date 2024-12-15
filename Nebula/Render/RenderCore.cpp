#include "RenderCore.h"
#include <iostream>

namespace nbl
{
	template<>
	nRenderModule& nModuleManager::LoadModule<nRenderModule>()
	{
		static nRenderModule msRenderModule;
		return msRenderModule;
	};
	void nRenderModule::UnitTest()
	{
		std::cout << __FUNCDNAME__;
	}
}


