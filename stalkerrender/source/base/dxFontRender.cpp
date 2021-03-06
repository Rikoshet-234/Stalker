#include "stdafx.h"
#include "dxFontRender.h"

#include "engine/GameFont.h"

dxFontRender::dxFontRender()
{

}

dxFontRender::~dxFontRender()
{
	pShader.destroy();
	pGeom.destroy();
}

void dxFontRender::Initialize(LPCSTR cShader, LPCSTR cTexture)
{
	pShader.create(cShader, cTexture);
	pGeom.create(FVF::F_TL, RCache.Vertex.Buffer(), RCache.QuadIB);
}
extern ENGINE_API BOOL g_bRendering;
extern ENGINE_API Fvector2		g_current_font_scale;
void dxFontRender::OnRender(CGameFont &owner)
{
	VERIFY				(g_bRendering);
	if (pShader)		RCache.set_Shader	(pShader);

	if (!(owner.uFlags&CGameFont::fsValid)){
		CTexture* T		= RCache.get_ActiveTexture(0);
		owner.vTS.set			((int)T->get_Width(),(int)T->get_Height());
		owner.fTCHeight		= owner.fHeight/float(owner.vTS.y);
		owner.uFlags			|= CGameFont::fsValid;
	}

	for (u32 i=0; i<owner.strings.size(); ){
		// calculate first-fit
		bsize		count	=	1;

		bsize length = owner.smart_strlen( owner.strings[ i ].string );

		while	((i+count)<owner.strings.size()) {
			bsize L = owner.smart_strlen( owner.strings[ i + count ].string );

			if ((L+length)<MAX_MB_CHARS){
				count	++;
				length	+=	L;
			}
			else		break;
		}

		// lock AGP memory
		bsize	vOffset;
		FVF::TL* v		= (FVF::TL*)RCache.Vertex.Lock	(length*4,pGeom.stride(),vOffset);
		FVF::TL* start	= v;

		// fill vertices
		bsize last		= i+count;
		for (; i<last; i++) {
			CGameFont::String		&PS	= owner.strings[i];
			wide_char wsStr[ MAX_MB_CHARS ];

			bsize	len	= owner.IsMultibyte() ?
				mbhMulti2Wide( wsStr , NULL , MAX_MB_CHARS , PS.string ) :
			xr_strlen( PS.string );

			if (len) {
				float	X	= float(XrMath::iFloor(PS.x));
				float	Y	= float(XrMath::iFloor(PS.y));
				float	S	= PS.height*g_current_font_scale.y;
				float	Y2	= Y+S;
				float fSize = 0;

				if ( PS.align )
					fSize = owner.IsMultibyte() ? owner.SizeOf_( wsStr ) : owner.SizeOf_( PS.string );

				switch ( PS.align )
				{
				case CGameFont::alCenter:	
					X	-= ( XrMath::iFloor( fSize * 0.5f ) ) * g_current_font_scale.x;	
					break;
				case CGameFont::alRight:	
					X	-=	XrMath::iFloor( fSize );
					break;
				}

				u32	clr,clr2;
				clr2 = clr	= PS.c;
				if (owner.uFlags&CGameFont::fsGradient){
					u32	_R	=XrColor::color_get_R	(clr)/2;
					u32	_G	=XrColor::color_get_G	(clr)/2;
					u32	_B	=XrColor::color_get_B	(clr)/2;
					u32	_A	=XrColor::color_get_A	(clr);
					clr2	=XrColor::color_rgba	(_R,_G,_B,_A);
				}

#if defined(USE_DX10) || defined(USE_DX11)		//	Vertex shader will cancel a DX9 correction, so make fake offset
				X			-= 0.5f;
				Y			-= 0.5f;
				Y2			-= 0.5f;
#endif	//	USE_DX10

				float	tu,tv;
				for (int j=0; j<len; j++)
				{
					Fvector l;

					l = owner.IsMultibyte() ? owner.GetCharTC( wsStr[ 1 + j ] ) : owner.GetCharTC( ( u16 ) ( u8 ) PS.string[j] );

					float scw		= l.z * g_current_font_scale.x;

					float fTCWidth	= l.z/owner.vTS.x;

					if (!XrMath::fis_zero(l.z))
					{
//						tu			= ( l.x / owner.vTS.x ) + ( 0.5f / owner.vTS.x );
//						tv			= ( l.y / owner.vTS.y ) + ( 0.5f / owner.vTS.y );
						tu			= ( l.x / owner.vTS.x );
						tv			= ( l.y / owner.vTS.y );
#ifndef	USE_DX10
						//	Make half pixel offset for 1 to 1 mapping
						tu			+=( 0.5f / owner.vTS.x );
						tv			+=( 0.5f / owner.vTS.y );
#endif	//	USE_DX10

						v->set( X , Y2 , clr2 , tu , tv + owner.fTCHeight );						v++;
						v->set( X ,	Y , clr , tu , tv );									v++;
						v->set( X + scw , Y2 , clr2 , tu + fTCWidth , tv + owner.fTCHeight );		v++;
						v->set( X + scw , Y , clr , tu + fTCWidth , tv );					v++;
					}
					X += scw * owner.vInterval.x;
					if ( owner.IsMultibyte() ) {
						X -= 2;
						if ( IsNeedSpaceCharacter( wsStr[ 1 + j ] ) )
							X += owner.fXStep;
					}
				}
			}
		}

		// Unlock and draw
		u32 vCount = (u32)(v-start);
		RCache.Vertex.Unlock		(vCount,pGeom.stride());
		if (vCount){
			RCache.set_Geometry		(pGeom);
			RCache.Render			(D3DPT_TRIANGLELIST,vOffset,0,vCount,0,vCount/2);
		}
	}
}