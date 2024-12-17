#pragma once
#include "RenderModuleDefines.h"

namespace nbl
{
	enum class nEnumRenderBackend
	{
		Vulkan,
		OpenGL
	};

	class RENDER_API nRHI
	{
	protected:
		nRHI();
	public:
		virtual ~nRHI();

		virtual nEnumRenderBackend GetType()const = 0;

		virtual bool InitBackend(void*)const = 0;
	};
}