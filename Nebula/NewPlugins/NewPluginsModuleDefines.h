#pragma once

#ifdef _WIN32
	#ifdef NEWPLUGINS_EXPORTS
		#define NEWPLUGINS_API __declspec(dllexport)
	#else
		#define NEWPLUGINS_API __declspec(dllimport)
	#endif
#elif defined(__linux__) || defined(__APPLE__)
	#ifdef NEWPLUGINS_EXPORTS
		#define NEWPLUGINS_API __attribute__ ((visibility("default")))
	#else
		#define NEWPLUGINS_API
	#endif
#else
	#define NEWPLUGINS_API
#endif
