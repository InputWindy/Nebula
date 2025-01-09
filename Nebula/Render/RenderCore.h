/*
	模块内头文件引用使用""
	模块外头文件引用使用<>

	以防出现跨模块同名文件引用错误导致报错
*/
#pragma once
#include "RenderModuleDefines.h"
#include <Module/ModuleManager.h>

#include "RHI/RHI.h"
#include "Window/PlatformWindow.h"

namespace nbl
{
	enum class nEnumSurfaceType
	{
		OffScreen,	/*no window*/
		Windowed,	/*window*/
	};

	/*
		有vulkan用vulkan，没vulkan用opengl做fallback
	*/
	enum class nEnumRenderFeatureLevel
	{
		GraphicOnly,		/*opengl < 4.3 or Vk > 1.0.0*/
		ComputeOnly,		/*opengl < 4.3 or Vk > 1.0.0*/
		GraphicAndCompute,	/*opengl >= 4.3 or Vk > 1.0.0*/
		PC,					/*opengl 4.6 or Vk 1.0.3*/
		Mobile				/*opengl < 4.3 or Vk 1.0.0*/
	};

	struct RENDER_API nRenderModuleCreateInfo
	{
		std::string Title;

		int W = 800;
		int H = 600;

		nEnumSurfaceType SurfaceType;
		bool  bResizable = true;

		nEnumRenderFeatureLevel FeatureLevel;

		void* UserData = nullptr;
	};

	class RENDER_API nRenderModule final: public nIModule
	{
		GENERATED_MODULE(nRenderModule)
	public:
		bool Init(const nRenderModuleCreateInfo&);

	public:
		/// <summary>
		/// check validation
		/// </summary>
		/// <returns></returns>
		bool IsValidPlatformWindow()const;

		/// <summary>
		/// get valid window
		/// </summary>
		/// <returns>p window</returns>
		nPlatformWindowAccessor GetPlatformWindowChecked();

		/// <summary>
		/// get raw ptr
		/// </summary>
		/// <returns>p window(can be null)</returns>
		nPlatformWindowAccessor GetPlatformWindow();

		/// <summary>
		/// get valid rhi
		/// </summary>
		/// <returns></returns>
		nRHIAccessor GetRHIChecked()const;

		/// <summary>
		/// get raw ptr
		/// </summary>
		/// <returns></returns>
		nRHIAccessor GetRHI()const;

		/// <summary>
		/// is valid rhi
		/// </summary>
		/// <returns></returns>
		bool  IsValidRHI()const;

	private:
		nRenderModuleCreateInfo CreateInfo;
		std::unique_ptr<nPlatformWindow> PlatformWindow;
		std::unique_ptr<nRHI> RenderInterface;
	};
}
