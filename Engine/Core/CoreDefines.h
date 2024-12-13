#pragma once

#ifdef _WIN32
	#ifdef ENGINE_EXPORTS
		#define ENGINE_API __declspec(dllexport)
	#else
		#define ENGINE_API __declspec(dllimport)
	#endif
#elif defined(__linux__) || defined(__APPLE__)
	#ifdef ENGINE_EXPORTS
		#define ENGINE_API __attribute__ ((visibility("default")))
	#else
		#define ENGINE_API
	#endif
#else
	#define ENGINE_API
#endif
