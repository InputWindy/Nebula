#pragma once
#include "RHI/RHI.h"

namespace nbl
{
	class RENDER_API nOpenGLRHI :public nRHI
	{
	public:
		nOpenGLRHI();
		virtual ~nOpenGLRHI();

		virtual nEnumRenderBackend GetType()const override final;

		virtual nEnumRHIInitResult InitBackend(nRHICreateInfo*) override final;

	private:

	};
}