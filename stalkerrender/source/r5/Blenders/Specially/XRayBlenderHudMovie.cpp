#include "pch.h"
#include "XRayBlenderHudMovie.h"

XRayBlenderHudMovie::XRayBlenderHudMovie()
{
}

XRayBlenderHudMovie::~XRayBlenderHudMovie()
{
}

void XRayBlenderHudMovie::Initialize()
{
	BearRootSignatureDescription RootSignatureDescription;
	RootSignatureDescription.Samplers[0].Shader = ST_Pixel;
	RootSignatureDescription.SRVResources[0].Shader = ST_Pixel;
	RootSignatureDescription.UniformBuffers[0].Shader = ST_Vertex;
	
	RootSignature[0] = BearRenderInterface::CreateRootSignature(RootSignatureDescription);

	BearPipelineDescription PipelineDescription;
	PipelineDescription.RenderPass = GRenderTarget->RenderPass_Base;
	CreatePipeline(0,PipelineDescription, "notransform", "yuv2rgb", SVD_TL);
}

void XRayBlenderHudMovie::Compile(XRayShaderElement& shader)
{
	if (IDShader == 0)
	{
		SetTexture(shader, 0, "$base0");
		shader.SamplerStates[0] = SSS_Default;
		shader.TypeTransformation = STT_Screen;
	}
}
