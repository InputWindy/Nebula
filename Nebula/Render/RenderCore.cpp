#include "RenderCore.h"
#include <iostream>

namespace nbl
{
	
	template<> 
	__declspec(dllexport) nRenderModule& nModuleManager::LoadModule < nRenderModule >()
	{
		static nRenderModule msRenderModule;
		return msRenderModule;
	};
}


