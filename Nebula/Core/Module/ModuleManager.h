#pragma once
#include "CoreDefines.h"
#include "Common/Common.h"

#include <string>
#include <memory>

#include <type_traits>

namespace nbl
{
	class nIModule
	{
	protected:
		nIModule(){};
	public:
		virtual ~nIModule() {};
	};

	template <typename TModule>
	concept ValidModule = requires{ std::is_base_of_v<nIModule, TModule> && !std::is_same_v<nIModule, TModule>; };

	class CORE_API nModuleManager:public nSingleton<nModuleManager>
	{
	public:
		template<ValidModule T>
		T& LoadModule();
	};
}



