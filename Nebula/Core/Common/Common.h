#pragma once
#include "CoreDefines.h"
#include <string>
#include <memory>

namespace nbl
{
	typedef unsigned int nResult;
	enum nEnumResult_: unsigned int
	{
		Success = 0,
		Fail = 1
	};
}

namespace nbl
{
#define IMPL_SINGLETON(Type)\
template<>\
Type& nSingleton<Type>::Get()\
{\
	static Type instance;\
	return instance;\
};

	template<typename T>
	class CORE_API nSingleton
	{
	protected:
		nSingleton() {}

	public:
		static T& Get() noexcept(std::is_nothrow_constructible<T>::value);

		virtual ~nSingleton() noexcept = default;
		nSingleton(const nSingleton&) = delete;
		nSingleton& operator=(const nSingleton&) = delete;
	};
}



