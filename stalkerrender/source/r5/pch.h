#include "tools/xrCore.h"
#include "BearGraphics.hpp"
#include "BearUI.hpp"
#include "api/StalkerAPI.h"
#include "engine/XRayExports.h"
#include "engine/render.h"
#include "engine/Properties.h"
#include "engine/vis_common.h"
#include "RenderFactory.h"
#include "xrRender/UISequenceVideoItem.h"
#include "xrRender/ConsoleRender.h"
#include "xrRender/StatGraphRender.h"
#include "xrRender/EnvironmentRender.h"
#include "xrRender/LensFlareRender.h"
#include "xrRender/RainRender.h"
#include "xrRender/ThunderboltDescRender.h"
#include "xrRender/ThunderboltRender.h"
#include  "xrRender/RenderDeviceRender.h"
#include "xrRender/StatsRender.h"
#include "xrRender/ApplicationRender.h"
#include "xrRender/WallMarkArray.h"
#include "xrRender/ObjectSpaceRender.h"
#include "xrRender/DrawUtils.h"
#include "xrRender/FontRender.h"
#include "xrRender/UIShader.h"
#include "xrRender/UIRender.h"
#include "xrRender/DebugRender.h"

#define REGISTER(name,byte,size,a1,a2) name=byte,
enum D3DVertexState
{
#include "D3D9VertexState.h"
};
#undef REGISTER
#include "Engine/stdafx.h"
#include "engine/igame_level.h"
#include "engine/Fmesh.h"




#include "xrRender/FVF.h"

#include "General/XrayHardware.h"
#include "Resources/XRayResource.h"
#include "Resources/XRayTexture.h"
#include "Resources/XRayBlender.h"
#include "Blenders/XRayBlenderCompiler.h"
#include "Resources/XRayResourcesManager.h"
#include "Resources/XRayBlender_inline.h"

#include "Resources/Visual/XRayRenderVisual.h"
#include "Resources/Visual/XRayFVisual.h"
#include "Resources/Visual/XRayFHierrarhyVisual.h"
#include "Resources/Visual/XRayFProgressive.h"
#include "Resources/XRaySkeletonX.h"
#include "Resources/Visual/XRayKinematics.h"
#include "Resources/Visual/XRayKinematicsAnimated.h"
#include "Resources/Visual/XRayFSkinned.h"
#include "Resources/Visual/XRayTreeVisual.h"
#include "Resources/XRayModelPool.h"



#include "Engine/XRayRSector.h"

#include "Engine/XRayRenderFactory.h"
#include "General/XRayRenderTarget.h"
#include "Engine/XRayRenderInterface.h"
#include "Engine/XRayUIRender.h"
#include "Engine/XRayDUInterface.h"
#include "Engine/XRayDebugRender.h"


#include "Engine/Factory/XRayApplicationRender.h"
#include "Engine/Factory/XRayConsoleRender.h"
#include "Engine/Factory/XRayEnvDescriptorMixerRender.h"
#include "Engine/Factory/XRayEnvDescriptorRender.h"
#include "Engine/Factory/XRayEnvironmentRender.h"
#include "Engine/Factory/XRayFlareRender.h"
#include "Engine/Factory/XRayFlareRender.h"
#include "Engine/Factory/XRayFontRender.h"
#include "Engine/Factory/XRayLensFlareRender.h"
#include "Engine/Factory/XRayObjectSpaceRender.h"
#include "Engine/Factory/XRayRainRender.h"
#include "Engine/Factory/XRayRenderDeviceRender.h"
#include "Engine/Factory/XRayStatGraphRender.h"
#include "Engine/Factory/XRayStatsRender.h"
#include "Engine/Factory/XRayThunderboltDescRender.h"
#include "Engine/Factory/XRayThunderboltRender.h"
#include "Engine/Factory/XRayUISequenceVideoItem.h"
#include "Engine/Factory/XRayUIShader.h"
#include "Engine/Factory/XRayWallMarkArray.h"

#include "Blenders/Blender_CLSID.h"
#include "Blenders/XRayBlenderDefault.h"
#include "Blenders/XRayBlenderDefaultAref.h"
#include "Blenders/XRayBlenderVertex.h"
#include "Blenders/XRayBlenderVertexAref.h"
#include "Blenders/XRayBlenderScreenSet.h"
#include "Blenders/XRayBlenderScreenGray.h"
#include "Blenders/XRayBlenderEditorSelection.h"
#include "Blenders/XRayBlenderEditorWire.h"
#include "Blenders/XRayBlenderLaEmB.h"
#include "Blenders/XRayBlenderLmEbB.h"
#include "Blenders/XRayBlenderBmmD.h"
#include "Blenders/XRayBlenderShWorld.h"
#include "Blenders/XRayBlenderBlur.h"
#include "Blenders/XRayBlenderModel.h"
#include "Blenders/XRayBlenderModelEbB.h"
#include "Blenders/XRayBlenderDetailStill.h"
#include "Blenders/XRayBlenderTree.h"
#include "Blenders/XRayBlenderParticle.h"

