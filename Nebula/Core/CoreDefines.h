#pragma once

/*
	class type:
	
	nI - interface class
	nT - template class
	n  - nebula common class
*/

#ifdef _WIN32
	#ifdef CORE_EXPORTS
		#define CORE_API __declspec(dllexport)
	#else
		#define CORE_API __declspec(dllimport)
	#endif
#elif defined(__linux__) || defined(__APPLE__)
	#ifdef CORE_EXPORTS
		#define CORE_API __attribute__ ((visibility("default")))
	#else
		#define CORE_API
	#endif
#else
	#define CORE_API
#endif
