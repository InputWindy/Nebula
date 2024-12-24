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
		InvalidGetProcCallback,
		InvalidPlatformWindow ,
		InavailableLayers 
	};

	class nPlatformWindowAccessor;

	struct RENDER_API nRHICreateInfo
	{
		nRHICreateInfo() {};
		nRHICreateInfo(const nPlatformWindowAccessor& InPlatformWindow) :PlatformWindow(InPlatformWindow){};

		nPlatformWindowAccessor PlatformWindow;
	};

	struct nVulkanCreateInfo :public nRHICreateInfo
	{
		nVulkanCreateInfo() {};
		nVulkanCreateInfo(const nPlatformWindowAccessor& InPlatformWindow) :nRHICreateInfo(InPlatformWindow) {};

		/*
			ÊµÀý²ã
		*/
		std::vector<const char*> AppLayers = { "VK_LAYER_KHRONOS_validation" };

#ifdef NDEBUG
		bool bEnableValidationLayers = false;
#else
		bool bEnableValidationLayers = true;
#endif 

		uint32_t VulkanVersion;/* = VK_API_VERSION_1_0;*/
	};

	struct nOpenGLCreateInfo :public nRHICreateInfo
	{
		nOpenGLCreateInfo() {};
		nOpenGLCreateInfo(const nPlatformWindowAccessor& InPlatformWindow) :nRHICreateInfo(InPlatformWindow) {};
	};

	class RENDER_API nRHI
	{
		friend class nRenderModule;
	protected:
		nRHI();

		virtual nEnumRHIInitResult InitBackend(nRHICreateInfo*);
	public:
		virtual ~nRHI();

		virtual nEnumRenderBackend GetType()const = 0;
	protected:
		nRHICreateInfo Info;
	};

	struct RENDER_API nRHIAccessor final :public nModulePtrAccessor<nRHI>
	{
		GENERATED_PTR_ACCESSOR(nRHI)
	};
}