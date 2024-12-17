#pragma once

/*
	class type:
	
	nI - interface class
	nT - template class
	nE - enum class
	n  - nebula common class
*/
/*
	模块内头文件引用使用""
	模块外头文件引用使用<>

	以防出现跨模块同名文件引用错误导致报错
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
