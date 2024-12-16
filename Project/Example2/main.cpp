#include <Application/Application.h>

#include <imgui.h>
#include <glad/glad.h>

struct Example2App :public nbl::nIApp
{
public:
	Example2App(int argc, char* argv[])
	{
		ParseCmdline(argc, argv);
	}

	void StartUp() override final
	{
		nIApp::StartUp();

		/*
			TODO: your initialization .
		*/
		{

		}
	};

	void Run() override final
	{

	}

	void Shutdown() override final
	{
		/*
			TODO: your shutdown .
		*/
		{

		}

		nIApp::Shutdown();
	};


protected:
	void ParseCmdline(int argc, char* argv[]) override final
	{
		
	}
};

IMPL_APP(Example2App)
#include <EntryPoint.h>
