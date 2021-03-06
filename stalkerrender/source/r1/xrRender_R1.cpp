// xrRender_R1.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "xrRender/dxRenderFactory.h"
#include "xrRender/dxUIRender.h"
#include "xrRender/dxDebugRender.h"
extern void setup_luabind_allocator();
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		::Render							= &RImplementation;
		::RenderFactory				= &RenderFactoryImpl;
		::DU						= &DUImpl;
		//::vid_mode_token			= inited by HW;
		UIRender					= &UIRenderImpl;
#ifdef DEBUG
		DRender						= &DebugRenderImpl;
#endif // DEBUG
		xrRender_initconsole				();
		FS.SubPath(TEXT("%cur_shaders%"));
		FS.SubPath(TEXT("%shaders_cache%"));
		FS.AppendPath(TEXT("%cur_shaders%"), ::Render->getShaderPath(), TEXT("%shaders%"), 0);
		FS.AppendPath(TEXT("%shaders_cache%"), ::Render->getShaderPath(), TEXT("%user%"), 0);
		FS.CreateDirectory(TEXT("%shaders_cache%"), 0);
		setup_luabind_allocator();
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
	
		break;
	case DLL_PROCESS_DETACH:
		
		break;
	}
    return TRUE;
}
