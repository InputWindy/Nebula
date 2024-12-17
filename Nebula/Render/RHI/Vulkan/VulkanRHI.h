#pragma once
#include "RHI/RHI.h"

namespace nbl
{
	class RENDER_API nVulkanRHI:public nRHI
	{
	public:
		nVulkanRHI();
		virtual ~nVulkanRHI();

		virtual nEnumRenderBackend GetType()const override final;

		virtual bool InitBackend(void*)const override final;
	private:

	};
}