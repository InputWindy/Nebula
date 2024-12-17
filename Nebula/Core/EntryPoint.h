#pragma once
#include "Application/Application.h"

#include <exception>
#include <stdexcept>
#include <iostream>

int main(int argc, char* argv[])
{
	try
	{
		nbl::nIApp* gApp = nbl::CreateApp(argc, argv);
		gApp->StartUp();
		gApp->Run();
		gApp->Shutdown();

		delete gApp;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		std::exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}