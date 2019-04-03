#include "pch.h"

XRayDUInterface::XRayDUInterface()
{
}

void __stdcall XRayDUInterface::DrawCross(const Fvector & p, float szx1, float szy1, float szz1, float szx2, float szy2, float szz2, u32 clr, BOOL bRot45)
{
	
}

void __stdcall XRayDUInterface::DrawCross(const Fvector & p, float sz, u32 clr, BOOL bRot45)
{
	
}

void __stdcall XRayDUInterface::DrawFlag(const Fvector & p, float heading, float height, float sz, float sz_fl, u32 clr, BOOL bDrawEntity)
{
	
}

void __stdcall XRayDUInterface::DrawRomboid(const Fvector & p, float radius, u32 clr)
{
	
}

void __stdcall XRayDUInterface::DrawJoint(const Fvector & p, float radius, u32 clr)
{
	
}

void __stdcall XRayDUInterface::DrawSpotLight(const Fvector & p, const Fvector & d, float range, float phi, u32 clr)
{
	
}

void __stdcall XRayDUInterface::DrawDirectionalLight(const Fvector & p, const Fvector & d, float radius, float range, u32 clr)
{
	
}

void __stdcall XRayDUInterface::DrawPointLight(const Fvector & p, float radius, u32 clr)
{
	
}

void __stdcall XRayDUInterface::DrawSound(const Fvector & p, float radius, u32 clr)
{
	
}

void __stdcall XRayDUInterface::DrawLineSphere(const Fvector & p, float radius, u32 clr, BOOL bCross)
{
	
}

void __stdcall XRayDUInterface::dbgDrawPlacement(const Fvector & p, int sz, u32 clr, LPCSTR caption, u32 clr_font)
{
	
}

void __stdcall XRayDUInterface::dbgDrawVert(const Fvector & p0, u32 clr, LPCSTR caption)
{
	
}

void __stdcall XRayDUInterface::dbgDrawEdge(const Fvector & p0, const Fvector & p1, u32 clr, LPCSTR caption)
{
	
}

void __stdcall XRayDUInterface::dbgDrawFace(const Fvector & p0, const Fvector & p1, const Fvector & p2, u32 clr, LPCSTR caption)
{
	
}

void __stdcall XRayDUInterface::DrawFace(const Fvector & p0, const Fvector & p1, const Fvector & p2, u32 clr_s, u32 clr_w, BOOL bSolid, BOOL bWire)
{
	
}

void __stdcall XRayDUInterface::DrawLine(const Fvector & p0, const Fvector & p1, u32 clr)
{
	
}

void __stdcall XRayDUInterface::DrawLink(const Fvector & p0, const Fvector & p1, float sz, u32 clr)
{
	
}

void __stdcall XRayDUInterface::DrawFaceNormal(const Fvector & p0, const Fvector & p1, const Fvector & p2, float size, u32 clr)
{
	
}

void __stdcall XRayDUInterface::DrawFaceNormal(const Fvector * p, float size, u32 clr)
{
	
}

void __stdcall XRayDUInterface::DrawFaceNormal(const Fvector & C, const Fvector & N, float size, u32 clr)
{
	
}

void __stdcall XRayDUInterface::DrawSelectionBox(const Fvector & center, const Fvector & size, u32 * c)
{
	
}

void __stdcall XRayDUInterface::DrawSelectionBoxB(const Fbox & box, u32 * c)
{
	
}

void __stdcall XRayDUInterface::DrawIdentSphere(BOOL bSolid, BOOL bWire, u32 clr_s, u32 clr_w)
{
	
}

void __stdcall XRayDUInterface::DrawIdentSpherePart(BOOL bSolid, BOOL bWire, u32 clr_s, u32 clr_w)
{
	
}

void __stdcall XRayDUInterface::DrawIdentCone(BOOL bSolid, BOOL bWire, u32 clr_s, u32 clr_w)
{
	
}

void __stdcall XRayDUInterface::DrawIdentCylinder(BOOL bSolid, BOOL bWire, u32 clr_s, u32 clr_w)
{
	
}

void __stdcall XRayDUInterface::DrawIdentBox(BOOL bSolid, BOOL bWire, u32 clr_s, u32 clr_w)
{
	
}

void __stdcall XRayDUInterface::DrawBox(const Fvector & offs, const Fvector & Size, BOOL bSolid, BOOL bWire, u32 clr_s, u32 clr_w)
{
	
}

void __stdcall XRayDUInterface::DrawAABB(const Fvector & p0, const Fvector & p1, u32 clr_s, u32 clr_w, BOOL bSolid, BOOL bWire)
{
	
}

void __stdcall XRayDUInterface::DrawAABB(const Fmatrix & parent, const Fvector & center, const Fvector & size, u32 clr_s, u32 clr_w, BOOL bSolid, BOOL bWire)
{
	
}

void __stdcall XRayDUInterface::DrawOBB(const Fmatrix & parent, const Fobb & box, u32 clr_s, u32 clr_w)
{
	
}

void __stdcall XRayDUInterface::DrawSphere(const Fmatrix & parent, const Fvector & center, float radius, u32 clr_s, u32 clr_w, BOOL bSolid, BOOL bWire)
{
	
}

void __stdcall XRayDUInterface::DrawSphere(const Fmatrix & parent, const Fsphere & S, u32 clr_s, u32 clr_w, BOOL bSolid, BOOL bWire)
{
	
}

void __stdcall XRayDUInterface::DrawCylinder(const Fmatrix & parent, const Fvector & center, const Fvector & dir, float height, float radius, u32 clr_s, u32 clr_w, BOOL bSolid, BOOL bWire)
{
	
}

void __stdcall XRayDUInterface::DrawCone(const Fmatrix & parent, const Fvector & apex, const Fvector & dir, float height, float radius, u32 clr_s, u32 clr_w, BOOL bSolid, BOOL bWire)
{
	
}

void __stdcall XRayDUInterface::DrawPlane(const Fvector & center, const Fvector2 & scale, const Fvector & rotate, u32 clr_s, u32 clr_w, BOOL bCull, BOOL bSolid, BOOL bWire)
{
	
}

void __stdcall XRayDUInterface::DrawPlane(const Fvector & p, const Fvector & n, const Fvector2 & scale, u32 clr_s, u32 clr_w, BOOL bCull, BOOL bSolid, BOOL bWire)
{
	
}

void __stdcall XRayDUInterface::DrawRectangle(const Fvector & o, const Fvector & u, const Fvector & v, u32 clr_s, u32 clr_w, BOOL bSolid, BOOL bWire)
{
	
}

void __stdcall XRayDUInterface::DrawGrid()
{
	
}

void __stdcall XRayDUInterface::DrawPivot(const Fvector & pos, float sz)
{
	
}

void __stdcall XRayDUInterface::DrawAxis(const Fmatrix & T)
{
	
}

void __stdcall XRayDUInterface::DrawObjectAxis(const Fmatrix & T, float sz, BOOL sel)
{
	
}

void __stdcall XRayDUInterface::DrawSelectionRect(const Ivector2 & m_SelStart, const Ivector2 & m_SelEnd)
{
	
}

void __stdcall XRayDUInterface::DrawIndexedPrimitive(int prim_type, u32 pc, const Fvector & pos, const Fvector * vb, const u32 & vb_size, const u32 * ib, const u32 & ib_size, const u32 & clr_argb, float scale)
{
	
}

void __stdcall XRayDUInterface::OutText(const Fvector & pos, LPCSTR text, u32 color, u32 shadow_color)
{
	
}

void __stdcall XRayDUInterface::OnDeviceDestroy()
{
	
}
