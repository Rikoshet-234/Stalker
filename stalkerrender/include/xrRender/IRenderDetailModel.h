#ifndef IRenderDetailModelH
#define IRenderDetailModelH
#pragma once

#include "RenderDetailModel.h"

//////////////////////////////////////////////////////////////////////////
// definition (Detail Model)
class		IRender_DetailModel
{
public:
	struct fvfVertexIn	{
		Fvector P;
		float	u,v;
	};
	struct fvfVertexOut	{
		Fvector P;
		u32		C;
		float	u,v;
	};
public:
	Fsphere		bv_sphere;
	Fbox		bv_bb;
	flags32		m_Flags;
	float		m_fMinScale;
	float		m_fMaxScale;

	ref_shader	shader;
	fvfVertexIn	*vertices;
	bsize			number_vertices;     
	u16			*indices;
	bsize			number_indices;
public:
	virtual void					transfer	(Fmatrix& mXform, fvfVertexOut* vDest, u32 C, u16* iDest, bsize iOffset)	= 0;
	virtual void					transfer	(Fmatrix& mXform, fvfVertexOut* vDest, u32 C, u16* iDest, bsize iOffset, float du, float dv)	= 0;
	virtual ~IRender_DetailModel()	{};
};

#endif