#include "stdafx.h"
#include "UIcursor.h"

#include "engine/CustomHUD.h"
#include "UI.h"
#include "HUDManager.h"
#include "UI/UIStatic.h"
ENGINE_API extern u32   size_screen_x, size_screen_y;

#define C_DEFAULT	D3DCOLOR_XRGB(0xff,0xff,0xff)

CUICursor::CUICursor()
:m_static(NULL)
{    
	bVisible				= false;
	vPos.set				(0.f,0.f);
	InitInternal			();
	Device.seqRender.Add	(this,2);
}
//--------------------------------------------------------------------
CUICursor::~CUICursor	()
{
	xr_delete				(m_static);
	Device.seqRender.Remove	(this);
}

void CUICursor::OnScreenRatioChanged()
{
	xr_delete					(m_static);
	InitInternal				();
}

void CUICursor::InitInternal()
{
	m_static					= xr_new<CUIStatic>();
	m_static->InitTextureEx		("ui\\ui_ani_cursor", "hud\\cursor");
	Frect						rect;
	rect.set					(0.0f,0.0f,40.0f,40.0f);
	m_static->SetOriginalRect	(rect);
	Fvector2					sz;
	sz.set						(rect.rb);
	if(UI()->is_16_9_mode())
		sz.x					/= 1.2f;

	m_static->SetWndSize		(sz);
	m_static->SetStretchTexture	(true);
	u32 screen_size_x = GetSystemMetrics(SM_CXSCREEN);
	u32 screen_size_y = GetSystemMetrics(SM_CYSCREEN);
	m_b_use_win_cursor = (screen_size_y >= Device.dwHeight && screen_size_x >= Device.dwWidth);
}

//--------------------------------------------------------------------
u32 last_render_frame = 0;
void CUICursor::OnRender	()
{
	if( !IsVisible() ) return;
#ifdef DEBUG
	VERIFY(last_render_frame != Device.dwFrame);
	last_render_frame = Device.dwFrame;

	if(bDebug)
	{
	CGameFont* F		= UI()->Font()->pFontDI;
	F->SetAligment		(CGameFont::alCenter);
	F->SetHeightI		(0.02f);
	F->OutSetI			(0.f,-0.9f);
	F->SetColor			(0xffffffff);
	Fvector2			pt = GetCursorPosition();
	F->OutNext			("%f-%f",pt.x, pt.y);
	}
#endif

	m_static->SetWndPos	(vPos);
	m_static->Update	();
	m_static->Draw		();
}

Fvector2 CUICursor::GetCursorPosition()
{
	return  vPos;
}

Fvector2 CUICursor::GetCursorPositionDelta()
{
	Fvector2 res_delta;

	res_delta.x = vPos.x - vPrevPos.x;
	res_delta.y = vPos.y - vPrevPos.y;
	return res_delta;
}

void CUICursor::UpdateCursorPosition()
{

	/*POINT		p;
	BOOL r		= GetCursorPos(&p);
	R_ASSERT	(r);

	vPrevPos = vPos;

	vPos.x			= (float)p.x * (UI_BASE_WIDTH/(float)Device.dwWidth);
	vPos.y			= (float)p.y * (UI_BASE_HEIGHT/(float)Device.dwHeight);
	clamp			(vPos.x, 0.f, UI_BASE_WIDTH);
	clamp			(vPos.y, 0.f, UI_BASE_HEIGHT);*/
	Fvector2	p;
	vPrevPos = vPos;
	if (m_b_use_win_cursor)
	{
		POINT point;
		GetCursorPos(&point);
		p.x = (float)point.x;
		p.y = (float)point.y;
		vPos.x += p.x - ((int)size_screen_x / 2);
		vPos.y += p.y - ((int)size_screen_y / 2);
	}
	else
	{	Ivector2 pti;
		IInputReceiver::IR_GetMousePosReal(pti);
		float sens = 1.0f;
		vPos.x += pti.x * sens;
		vPos.y += pti.y * sens;
	}
	clamp(vPos.x, 0.f, UI_BASE_WIDTH);
	clamp(vPos.y, 0.f, UI_BASE_HEIGHT);
}

void CUICursor::SetUICursorPosition(Fvector2 pos)
{
	vPos		= pos;
	POINT		p;
	p.x			= iFloor(vPos.x / (UI_BASE_WIDTH/(float)Device.dwWidth));
	p.y			= iFloor(vPos.y / (UI_BASE_HEIGHT/(float)Device.dwHeight));
	if (m_b_use_win_cursor)
		ClientToScreen(Device.m_hWnd, (LPPOINT)&p);
	SetCursorPos(p.x, p.y);
}