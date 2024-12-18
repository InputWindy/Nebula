#pragma once
#include "CoreDefines.h"
#include "Common/Common.h"

#include <string>
#include <memory>
#include <vector>
#include <map>

#include <type_traits>

#define GENERATED_MODULE(Type)\
friend class nModuleManager;\
public:\
static inline std::string GetModuleName() { return #Type; }\
protected:\
virtual bool StartupModule();\
virtual bool ShutdownModule();\
protected:\
Type() {}\
public:\
virtual ~Type() {}

#define IMPL_MODULE(API,Type) \
extern template API Type& nModuleManager::LoadModule<Type>();\
template<>\
Type& nModuleManager::LoadModule<Type>()\
{\
	auto Iter = LoadedModules.find(Type::GetModuleName());\
	if (Iter == LoadedModules.end())\
	{\
		nIModule* NewModule = new Type();\
		NewModule->StartupModule();\
		LoadedModules.insert(std::pair<std::string, std::unique_ptr<nIModule>>{ Type::GetModuleName(), NewModule });\
	}\
	return *reinterpret_cast<Type*>(LoadedModules[Type::GetModuleName()].get());\
};

namespace nbl
{
	class CORE_API nIModule
	{
		GENERATED_MODULE(nIModule)
	};
}

namespace nbl
{
	template <typename TModule>
	concept ValidModule = requires{ std::is_base_of_v<nIModule, TModule> && !std::is_same_v<nIModule, TModule>; };

	class CORE_API nModuleManager :public nSingleton<nModuleManager>
	{
	public:
		template<ValidModule T>
		T& LoadModule();

		bool ShutdownModules();

	protected:
		static inline std::map<std::string, std::unique_ptr<nIModule>> LoadedModules;
	};

	template<typename T>
	class CORE_API nModulePtrAccessor
	{
	public:
		nModulePtrAccessor(T* InPtr) :Ptr(InPtr) {}
		nModulePtrAccessor(const nModulePtrAccessor& Other) :Ptr(Other.Ptr) {}

		void operator=(const nModulePtrAccessor& Other)noexcept
		{
			Ptr = Other.Ptr;
		}

		virtual ~nModulePtrAccessor() {};

	protected:
		T* Ptr;
	};
}


