#pragma once
#include "CoreDefines.h"

#include <type_traits>

namespace nbl
{
	class nIApp;
	template <typename TApp>
	concept ValidApp = requires{ std::is_base_of_v<nIApp, TApp> && !std::is_same_v<nIApp, TApp>; };

	class CORE_API nIApp
	{
	protected:
		nIApp();
	public:
		virtual ~nIApp();

		virtual void StartUp();

		virtual void Run() = 0;

		virtual void Shutdown();
	protected:
		virtual void ParseCmdline(int argc, char* argv[]) = 0;
	};

	extern nIApp* CreateApp(int argc, char* argv[]);

#define IMPL_APP(AppType) \
nbl::nIApp* nbl::CreateApp(int argc, char* argv[])\
{\
	return nbl::ValidApp<AppType> ? new AppType(argc, argv) : nullptr;\
};

}
