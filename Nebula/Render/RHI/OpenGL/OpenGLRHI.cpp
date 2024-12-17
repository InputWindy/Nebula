#include "OpenGLRHI.h"

#include <glad/glad.h>
#include <cassert>
#include <exception>
#include <stdexcept>

nbl::nOpenGLRHI::nOpenGLRHI()
{
}

nbl::nOpenGLRHI::~nOpenGLRHI()
{
}

nbl::nEnumRenderBackend nbl::nOpenGLRHI::GetType() const
{
	return nEnumRenderBackend::OpenGL;
}

bool nbl::nOpenGLRHI::InitBackend(void* CallBack) const
{
	bool ret = gladLoadGLLoader((GLADloadproc)CallBack);

	if (!ret)
	{
		throw std::runtime_error("failed to load opengl,no gpu");
	}

	return ret;
}
