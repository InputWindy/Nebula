#pragma once

#ifdef _WIN32
	#ifdef RENDER_EXPORTS
		#define RENDER_API __declspec(dllexport)
	#else
		#define RENDER_API __declspec(dllimport)
	#endif
#elif defined(__linux__) || defined(__APPLE__)
	#ifdef RENDER_EXPORTS
		#define RENDER_API __attribute__ ((visibility("default")))
	#else
		#define RENDER_API
	#endif
#else
	#define RENDER_API
#endif
