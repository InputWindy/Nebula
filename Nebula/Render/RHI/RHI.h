#pragma once
#include "RenderModuleDefines.h"

#include <Common/Common.h>
#include <vector>
#include <string>
#include <Window/PlatformWindow.h>

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

	class nPlatformWindow;

	struct RENDER_API nRHICreateInfo
	{
		/*
			Ð£Ñé²ã
		*/
		std::vector<const char*> ValidationLayers = { "VK_LAYER_KHRONOS_validation" };

#ifdef NDEBUG
		bool bEnableValidationLayers = false;
#else
		bool bEnableValidationLayers = true;
#endif // NDEBUG

		std::string AppName;
		uint32_t AppVersion;
		std::string EngineName;
		uint32_t EngineVersion;
		uint32_t VulkanVersion;

		nPlatformWindowAccessor PlatformWindow;

		nEnumRenderBackend BackendType;
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
}