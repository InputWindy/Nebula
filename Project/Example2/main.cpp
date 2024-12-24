#include <Nebula.h>

#define VULKAN_BACKEND

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
		nbl::nPlatformWindowCreateInfo Info;
		Info.bHideWindow = false;
		Info.bResizable = true;
		Info.H = 600;
		Info.W = 800;
		Info.Title = "Example2";
		Info.UserData = this;

#ifdef VULKAN_BACKEND
		Info.RenderBackend = nbl::nEnumRenderBackend::Vulkan;
#else
		Info.RenderBackend = nbl::nEnumRenderBackend::OpenGL;
#endif
		
		if (RenderModule.CreatePlatformWindow(Info))
		{
			nbl::nPlatformWindowAccessor Window = RenderModule.GetPlatformWindowChecked();

#ifdef VULKAN_BACKEND
			nbl::nVulkanCreateInfo CreateInfo(Window);
#else
			nbl::nRHICreateInfo CreateInfo(Window);
#endif
			
			if (RenderModule.CreateRHI(&CreateInfo))
			{
				std::cout << "ok" << std::endl;
			}
			
			while (!Window.ShouldClose())
			{
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
