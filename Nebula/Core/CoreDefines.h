#pragma once

#ifdef _WIN32
	#ifdef NEBULA_EXPORTS
		#define NEBULA_API __declspec(dllexport)
	#else
		#define NEBULA_API __declspec(dllimport)
	#endif
#elif defined(__linux__) || defined(__APPLE__)
	#ifdef NEBULA_EXPORTS
		#define NEBULA_API __attribute__ ((visibility("default")))
	#else
		#define NEBULA_API
	#endif
#else
	#define NEBULA_API
#endif
