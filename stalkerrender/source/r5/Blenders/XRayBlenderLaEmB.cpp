#include "pch.h"

XRayBlenderLaEmB::XRayBlenderLaEmB()
{
	Description.ID = B_LaEmB;
	BearString::Copy(oT2_Name, TEXT("$null"));
	BearString::Copy(oT2_xform, TEXT("$null"));
	BearString::Copy(oT2_const, TEXT("$null"));
}

XRayBlenderLaEmB::~XRayBlenderLaEmB()
{
}

LPCSTR XRayBlenderLaEmB::getComment()
{
	return LPCSTR();
}

void XRayBlenderLaEmB::Save(IWriter & fs)
{
}

void XRayBlenderLaEmB::Load(IReader & fs, u16 version)
{
	XRayBlenderCompiler::Load(fs, version);
	xrPREAD_MARKER(fs);
	xrPREAD_PROP(fs, xrPID_TEXTURE, oT2_Name);
	xrPREAD_PROP(fs, xrPID_MATRIX, oT2_xform);
	xrPREAD_PROP(fs, xrPID_CONSTANT, oT2_const);
}

void XRayBlenderLaEmB::Initialize()
{
	BearRootSignatureDescription RootSignatureDescription;
	RootSignatureDescription.Samplers[0].Shader = ST_Pixel;
	RootSignatureDescription.SRVResources[0].Shader = ST_Pixel;
	RootSignatureDescription.UniformBuffers[0].Shader = ST_Vertex;
	RootSignature[0] = BearRenderInterface::CreateRootSignature(RootSignatureDescription);

	BearPipelineDescription PipelineDescription;
	PipelineDescription.DepthStencilState.DepthEnable = true;
	PipelineDescription.RenderPass = GRenderTarget->RenderPass_Base;
	CreatePipeline(0, PipelineDescription, "default", "default_tl", SVD_R1Vert);
}

void XRayBlenderLaEmB::Compile(XRayShaderElement& shader)
{
	if (IDShader == 0)
	{
		SetTexture(shader, 0, "$base0");
		shader.SamplerStates[0] = SSS_Default;
		shader.TypeTransformation = STT_Matrix;

	}
}

