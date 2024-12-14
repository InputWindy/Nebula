#pragma once
#include "CoreDefines.h"
#include <string>
#include <memory>

namespace nbl
{
	template<typename T>
	class CORE_API nSingleton
	{
	protected:
		nSingleton() {}

	public:
		static T& Get() noexcept(std::is_nothrow_constructible<T>::value)
		{
			static T instance;
			return instance;
		}
		virtual ~nSingleton() noexcept = default;
		nSingleton(const nSingleton&) = delete;
		nSingleton& operator=(const nSingleton&) = delete;
	};
}



