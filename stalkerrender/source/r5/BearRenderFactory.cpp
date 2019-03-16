#include "pch.h"
#undef RENDER_FACTORY_INTERFACE
#define RENDER_FACTORY_INTERFACE(Class)\
I ## Class* BearRenderFactory::Create ## Class()\
{\
	return static_cast<I ## Class*>(BearCore::bear_new<Bear ## Class>());\
}\
void BearRenderFactory::Destroy##Class(I ## Class *pObject)\
{\
}

#ifndef _EDITOR
RENDER_FACTORY_INTERFACE(UISequenceVideoItem)
RENDER_FACTORY_INTERFACE(UIShader)
RENDER_FACTORY_INTERFACE(StatGraphRender)
RENDER_FACTORY_INTERFACE(ConsoleRender)
RENDER_FACTORY_INTERFACE(RenderDeviceRender)
#	ifdef DEBUG
RENDER_FACTORY_INTERFACE(ObjectSpaceRender)
#	endif // DEBUG
RENDER_FACTORY_INTERFACE(ApplicationRender)
RENDER_FACTORY_INTERFACE(WallMarkArray)
RENDER_FACTORY_INTERFACE(StatsRender)
#endif // _EDITOR

#ifndef _EDITOR
RENDER_FACTORY_INTERFACE(EnvironmentRender)
RENDER_FACTORY_INTERFACE(EnvDescriptorMixerRender)
RENDER_FACTORY_INTERFACE(EnvDescriptorRender)
RENDER_FACTORY_INTERFACE(RainRender)
RENDER_FACTORY_INTERFACE(LensFlareRender)
RENDER_FACTORY_INTERFACE(ThunderboltRender)
RENDER_FACTORY_INTERFACE(ThunderboltDescRender)
RENDER_FACTORY_INTERFACE(FlareRender)
#endif // _EDITOR
RENDER_FACTORY_INTERFACE(FontRender)