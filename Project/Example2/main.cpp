#include <Application/Application.h>

#include <PlatformCore.h>

struct Example2App :public nbl::nIApp
{
public:
	Example2App(int argc, char* argv[])
	{
		ParseCmdline(argc, argv);
	}

	void Run() override final
	{
		auto& PlatformModule = nbl::nModuleManager::Get().LoadModule<nbl::nPlatformModule>();

		nbl::nPlatformWindowCreateInfo Info;
		Info.bHideWindow = false;
		Info.bResizable = true;
		Info.H = 600;
		Info.W = 800;
		Info.Title = "Example2";
		Info.UserData = this;

		if (PlatformModule.CreatePlatformWindow(Info, nbl::nEnumWindowBackend::Glfw))
		{
			nbl::nPlatformWindow* Window = PlatformModule.GetPlatformWindowChecked();
			while (!Window->ShouldClose())
			{
				Window->PollEvent();
			}
		};
	}



protected:
	void ParseCmdline(int argc, char* argv[]) override final
	{
		
	}
};

IMPL_APP(Example2App)
#include <EntryPoint.h>
