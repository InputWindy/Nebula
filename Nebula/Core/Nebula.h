#pragma once
#include <Application/Application.h>
#include <Common/Common.h>
#include <Module/ModuleManager.h>



#ifdef PLATFORM_MODULE
	#include <PlatformCore.h>
#endif

#ifdef RENDER_MODULE
	#include <RenderCore.h>
#endif

/*
	3rdparty
*/
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

#include <argparse/argparse.hpp>
#include <vulkan/vulkan.h>



