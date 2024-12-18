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
		auto& PlatformModule = nbl::nModuleManager::Get().LoadModule<nbl::nPlatformModule>();

		nbl::nPlatformWindowCreateInfo Info;
		Info.bHideWindow = false;
		Info.bResizable = true;
		Info.H = 600;
		Info.W = 800;
		Info.Title = "Example2";
		Info.UserData = this;
		Info.BackendType = nbl::nEnumWindowBackend::Glfw;

#ifdef VULKAN_BACKEND
		Info.bOpenGLBackend = false;
#else
		Info.bOpenGLBackend = true;
#endif
		
		if (PlatformModule.CreatePlatformWindow(Info))
		{
			auto& RenderModule = nbl::nModuleManager::Get().LoadModule<nbl::nRenderModule>();

			nbl::nPlatformWindowAccessor Window = PlatformModule.GetPlatformWindowChecked();

			nbl::nRHICreateInfo CreateInfo;
			CreateInfo.PlatformWindow = Window;
			CreateInfo.AppName = "Example2";
			CreateInfo.AppVersion = VK_MAKE_VERSION(1, 0, 0);
			CreateInfo.bEnableValidationLayers = true;
			CreateInfo.EngineName = "NoEngine";
			CreateInfo.EngineVersion = VK_MAKE_VERSION(1, 0, 0);
			CreateInfo.VulkanVersion = VK_API_VERSION_1_0;

#ifdef VULKAN_BACKEND
			CreateInfo.BackendType = nbl::nEnumRenderBackend::Vulkan;
#else
			CreateInfo.BackendType = nbl::nEnumRenderBackend::OpenGL;
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
