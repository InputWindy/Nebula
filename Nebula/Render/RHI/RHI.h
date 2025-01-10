#pragma once
#include "RenderModuleDefines.h"

#include <Module/ModuleManager.h>

#include <Common/Common.h>
#include <vector>
#include <string>
#include "Window/PlatformWindow.h"

namespace nbl
{
	class nRenderModule;

	enum class nEnumRenderBackend
	{
		Vulkan,
		OpenGL
	};

	enum class nEnumRHIInitResult :int
	{
		Success,

		EmptyCreateInfo,
		InvalidPlatformWindow,
		InvalidGetProcCallback,
		UnavailableLayers,
		InvalidVkInstance,
		InvalidSurface,
		NoSuitableGPU,
		InvalidLogicDevice,
		InvalidSwapChain,
		InvalidImageViews
	};

	enum class nEnumRenderFeatureLevel;
	enum class nEnumSurfaceType;

	struct RENDER_API nRHICreateInfo
	{
		nRHICreateInfo() {};
		nRHICreateInfo(const nPlatformWindowAccessor& InPlatformWindow) :PlatformWindow(InPlatformWindow){};

		nPlatformWindowAccessor PlatformWindow;

#ifdef NDEBUG
		bool bEnableValidationLayer = false;
#else
		bool bEnableValidationLayer = true;
#endif 
		nEnumRenderFeatureLevel FeatureLevel;
		nEnumSurfaceType SurfaceType;

		std::string AppName = "";
		int W = 800;
		int H = 600;
	};


	class RENDER_API nRHI
	{
		friend class nRenderModule;
		friend class nPlatformWindow;
	protected:
		nRHI();

		virtual nEnumRHIInitResult InitBackend(nRHICreateInfo*);
	public:
		virtual ~nRHI();

		/*
			rhi backend type
		*/
		virtual nEnumRenderBackend GetType()const = 0;

		/*
			support display on the screen
		*/
		virtual bool IsDisplaySupported()const;

		/*
			support hardware render pipeline
		*/
		virtual bool IsGraphicSupported()const;

		/*
			support compute shader
		*/
		virtual bool IsComputeSupported()const;

		/*
			support feature level
		*/
		virtual bool IsValid()const;
		
	public:
		const auto& GetCreateInfo()			const;
	protected:
		nRHICreateInfo Info;
	};

	struct RENDER_API nRHIAccessor final :public nModulePtrAccessor<nRHI>
	{
		GENERATED_PTR_ACCESSOR(nRHI)

		void ClearColor();
	};
}