#pragma once

#ifdef _WIN32
	#ifdef PLATFORM_EXPORTS
		#define PLATFORM_API __declspec(dllexport)
	#else
		#define PLATFORM_API __declspec(dllimport)
	#endif
#elif defined(__linux__) || defined(__APPLE__)
	#ifdef PLATFORM_EXPORTS
		#define PLATFORM_API __attribute__ ((visibility("default")))
	#else
		#define PLATFORM_API
	#endif
#else
	#define PLATFORM_API
#endif
