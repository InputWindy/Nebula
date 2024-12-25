#include <Nebula.h>


struct Example2App :public nbl::nIApp
{
public:
	Example2App(int argc, char* argv[])
	{
		ParseCmdline(argc, argv);
	}

	void Run() override final
	{
		auto& RenderModule = nbl::nModuleManager::Get().LoadModule<nbl::nRenderModule>();
	
		nbl::nRenderModuleCreateInfo Info;
		Info.SurfaceType = nbl::nEnumSurfaceType::Windowed;
		Info.bResizable = true;
		Info.W = 800;
		Info.H = 600;
		Info.Title = "Example2";
		Info.FeatureLevel = nbl::nEnumRenderFeatureLevel::Default;
		Info.UserData = this;

		if (RenderModule.Init(Info))
		{
			nbl::nPlatformWindowAccessor Window = RenderModule.GetPlatformWindowChecked();
			nbl::nRHIAccessor RHI = RenderModule.GetRHIChecked();

			while (!Window.ShouldClose())
			{
				RHI.ClearColor();

				Window.PollEvent();
			}
		};
	}



protected:
	void ParseCmdline(int argc, char* argv[]) override final
	{
		
	}
private:

};

IMPL_APP(Example2App)
#include <EntryPoint.h>
