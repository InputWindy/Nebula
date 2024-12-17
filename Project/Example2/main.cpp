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

#ifdef VULKAN_BACKEND
		Info.bOpenGLBackend = false;
#else
		Info.bOpenGLBackend = true;
#endif
		
		/*uint32_t ExtensionCount;
		vkEnumerateInstanceExtensionProperties(nullptr, &ExtensionCount, nullptr);

		std::cout << "ExtensionCount:" << ExtensionCount << std::endl;*/

		if (PlatformModule.CreatePlatformWindow(Info, nbl::nEnumWindowBackend::Glfw))
		{
			nbl::nPlatformWindow* Window = PlatformModule.GetPlatformWindowChecked();

			auto& RenderModule = nbl::nModuleManager::Get().LoadModule<nbl::nRenderModule>();
#ifdef VULKAN_BACKEND
			if (RenderModule.CreateRHI(nbl::nEnumRenderBackend::Vulkan))
#else
			if (RenderModule.CreateRHI(nbl::nEnumRenderBackend::OpenGL))
#endif
			{
				if (RenderModule.GetRHIChecked()->InitBackend(Window->GetProcAddressCallbackFunc()))
				{
					std::cout << "success." << std::endl;
				}
				else
				{
					std::cout << "fail." << std::endl;
				}
			}

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
private:

};

IMPL_APP(Example2App)
#include <EntryPoint.h>
