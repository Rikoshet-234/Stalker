#include "stdafx.h"

#include "dxApplicationRender.h"
#include "engine/x_ray.h"
#include "engine/GameFont.h"

void draw_multiline_text(CGameFont* F, float fTargetWidth, LPCSTR pszText);

void dxApplicationRender::Copy(IApplicationRender &_in)
{
	*this = *(dxApplicationRender*)&_in;
}

void dxApplicationRender::LoadTitleInt(LPCSTR str)
{
}

void dxApplicationRender::LoadBegin()
{
	ll_hGeom.create		(FVF::F_TL, RCache.Vertex.Buffer(), RCache.QuadIB);
	sh_progress.create	("hud\\default","ui\\ui_actor_loadgame_screen");
	hLevelLogo_Add.create	("hud\\default","ui\\ui_actor_widescreen_sidepanels.dds");

	ll_hGeom2.create		(FVF::F_TL, RCache.Vertex.Buffer(),NULL);
}

void dxApplicationRender::destroy_loading_shaders()
{
	hLevelLogo.destroy		();
	sh_progress.destroy		();
	hLevelLogo_Add.destroy	();
}

void dxApplicationRender::setLevelLogo(LPCSTR pszLogoName)
{
	hLevelLogo.create("hud\\default", pszLogoName);
}

void dxApplicationRender::KillHW()
{
	ZeroMemory(&HW,sizeof(CHW));
}

u32 calc_progress_color(u32, u32, int, int);

void dxApplicationRender::load_draw_internal(CApplication &owner)
{
#if defined(USE_DX10) || defined(USE_DX11)
	//	TODO: DX10: remove this???
	RImplementation.rmNormal();
	RCache.set_RT(HW.pBaseRT);
	RCache.set_ZB(HW.pBaseZB);
#endif	//	USE_DX10

#if defined(USE_DX10) || defined(USE_DX11)
	FLOAT ColorRGBA[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	HW.pContext->ClearRenderTargetView( RCache.get_RT(), ColorRGBA);
#else	//	USE_DX10
	CHK_DX			(HW.pDevice->Clear(0,0,D3DCLEAR_TARGET,D3DCOLOR_ARGB(0,0,0,0),1,0));
#endif	//	USE_DX10

	if(!sh_progress)
	{
		return;
	}

#if defined(USE_DX10) || defined(USE_DX11)
	//	TODO: DX10: remove this
//	FLOAT ColorRGBA[4] = {0.0f, 0.0f, 1.0f, 0.0f};
//	HW.pContext->ClearRenderTargetView( RCache.get_RT(), ColorRGBA);
//	HW.pContext->ClearDepthStencilView( RCache.get_ZB(), D3D_CLEAR_DEPTH|D3D_CLEAR_STENCIL, 1.0f, 0);
#endif	//	USE_DX10

	float	_w					= (float)Device.dwWidth;
	float	_h					= (float)Device.dwHeight;
	bool	b_ws				= (_w/_h) > 1.34f;
	bool	b_16x9				= b_ws && ((_w/_h)>1.77f);
	float	ws_k				= (b_16x9) ? 0.75f : 0.8333f;	//16:9 or 16:10
	float	ws_w				= b_ws ? (b_16x9?171.0f:102.6f) : 0.0f;

	float bw					= 1024.0f;
	float bh					= 768.0f;
	Fvector2					k; 
	k.set						(_w/bw, _h/bh);

	Fvector2					tsz;
	tsz.set						(1024, 1024);
	Frect						back_tex_coords;
	Frect						back_coords;
	Fvector2					back_size;
	Fvector2					back_tex_size;

	static float offs			= -0.5f;

	Fvector2					back_offset;
	if(b_ws)
		back_offset.set			(ws_w*ws_k, 0.0f); //ws_w == 171
	else
		back_offset.set			(0.0f, 0.0f);

	//progress bar
	
	back_tex_size.set			(506,4);
	back_size.set				(506,4);
	if(b_ws)
		back_size.x				*= ws_k; //ws

	back_tex_coords.lt.set		(0,772);
	back_tex_coords.rb.add		(back_tex_coords.lt, back_tex_size);

	back_coords.lt.set			(260,599);
	if(b_ws)
		back_coords.lt.x		*= ws_k;
	back_coords.lt.add			(back_offset);

	back_coords.rb.add			(back_coords.lt, back_size);
	back_coords.lt.mul			(k);
	back_coords.rb.mul			(k);

	back_tex_coords.lt.x		/= tsz.x; 
	back_tex_coords.lt.y		/= tsz.y; 
	back_tex_coords.rb.x		/= tsz.x; 
	back_tex_coords.rb.y		/= tsz.y;

	u32	Offset;
	u32	C						= 0xffffffff;
	FVF::TL* pv					= NULL;
	u32 v_cnt					= 40;
	pv							= (FVF::TL*)RCache.Vertex.Lock	(2*(v_cnt+1),ll_hGeom2.stride(),Offset);
	FVF::TL* _pv				= pv;
	float pos_delta				= back_coords.width()/v_cnt;
	float tc_delta				= back_tex_coords.width()/v_cnt;
	u32 clr = C;

	for(u32 idx=0; idx<v_cnt+1; ++idx)
	{
		clr =					calc_progress_color						(idx,v_cnt,owner.load_stage,owner.max_load_stage);
		pv->set					(back_coords.lt.x+pos_delta*idx+offs,	back_coords.rb.y+offs,	0+XrMath::EPS_S, 1, clr, back_tex_coords.lt.x+tc_delta*idx,	back_tex_coords.rb.y);	pv++;
		pv->set					(back_coords.lt.x+pos_delta*idx+offs,	back_coords.lt.y+offs,	0+XrMath::EPS_S, 1, clr, back_tex_coords.lt.x+tc_delta*idx,	back_tex_coords.lt.y);	pv++;
	}
	VERIFY						(u32(pv-_pv)==2*(v_cnt+1));
	RCache.Vertex.Unlock		(2*(v_cnt+1),ll_hGeom2.stride());

	RCache.set_Shader			(sh_progress);
	RCache.set_Geometry			(ll_hGeom2);
	RCache.Render				(D3DPT_TRIANGLESTRIP, Offset, 2*v_cnt);

	//background picture

	back_tex_size.set			(1024,768);
	back_size.set				(1024,768);
	if(b_ws)
		back_size.x				*= ws_k; //ws

	back_tex_coords.lt.set		(0,0);
	back_tex_coords.rb.add		(back_tex_coords.lt, back_tex_size);

	back_coords.lt.set			(offs, offs); 
	back_coords.lt.add			(back_offset); 
	back_coords.rb.add			(back_coords.lt, back_size);

	back_coords.lt.mul			(k);
	back_coords.rb.mul			(k);
	draw_face					(sh_progress, back_coords, back_tex_coords,tsz);

	if(b_ws) //draw additional frames (left&right)
	{
		//left
		back_size.set				(ws_w*ws_k, 768.0f);

		if(b_16x9)
		{
			back_tex_coords.lt.set		(0, 0);
			back_tex_coords.rb.set		(128, 768);
		}else
		{
			back_tex_coords.lt.set		(0, 0);
			back_tex_coords.rb.set		(128, 768);
		}
		back_coords.lt.set			(offs, offs); 
		back_coords.rb.add			(back_coords.lt, back_size);
		back_coords.lt.mul			(k);
		back_coords.rb.mul			(k);

		draw_face					(hLevelLogo_Add, back_coords, back_tex_coords,tsz);

		//right
		if(b_16x9)
		{
			back_tex_coords.lt.set		(128, 0);
			back_tex_coords.rb.set		(256, 768);
		}else
		{
			back_tex_coords.lt.set		(128, 0);
			back_tex_coords.rb.set		(256, 768);
		}

		back_coords.lt.set			(1024.0f-back_size.x+offs, offs); 
		back_coords.rb.add			(back_coords.lt, back_size);
		back_coords.lt.mul			(k);
		back_coords.rb.mul			(k);

		draw_face					(hLevelLogo_Add, back_coords, back_tex_coords, tsz);
	}


	// Draw title
	VERIFY							(owner.pFontSystem);
	owner.pFontSystem->Clear		();
	owner.pFontSystem->SetColor		(XrColor::color_rgba(103,103,103,255));
	owner.pFontSystem->SetAligment	(CGameFont::alCenter);
	back_size.set					(_w/2,622.0f*k.y);
	owner.pFontSystem->OutSet		(back_size.x, back_size.y);
	owner.pFontSystem->OutNext		(owner.ls_header);
	owner.pFontSystem->OutNext		("");
	owner.pFontSystem->OutNext		(owner.ls_tip_number);

	float fTargetWidth				= 600.0f*k.x*(b_ws?0.8f:1.0f);
	draw_multiline_text				(owner.pFontSystem, fTargetWidth, owner.ls_tip);

	owner.pFontSystem->OnRender		();

	//draw level-specific screenshot
	if(hLevelLogo)
	{
		Frect						r;
		r.lt.set					(0,173);

		if(b_ws)
			r.lt.x					*= ws_k;
		r.lt.add					(back_offset);

		r.lt.x						+= offs;
		r.lt.y						+= offs;
		back_size.set				(1024,399);

		if(b_ws)
			back_size.x				*= ws_k; //ws 0.625

		r.rb.add					(r.lt,back_size);
		r.lt.mul					(k);						
		r.rb.mul					(k);						
		Frect						logo_tex_coords;
		logo_tex_coords.lt.set		(0,0);
		logo_tex_coords.rb.set		(1.0f,0.77926f);

		draw_face					(hLevelLogo, r, logo_tex_coords, Fvector2().set(1,1));
	}
}

void dxApplicationRender::draw_face(ref_shader& sh, Frect& coords, Frect& tex_coords, const Fvector2& tsz)
{
	u32	Offset;
	u32	C						= 0xffffffff;
	FVF::TL* pv					= NULL;

	tex_coords.lt.x				/= tsz.x; 
	tex_coords.lt.y				/= tsz.y; 
	tex_coords.rb.x				/= tsz.x; 
	tex_coords.rb.y				/= tsz.y;

	pv							= (FVF::TL*) RCache.Vertex.Lock(4,ll_hGeom.stride(),Offset);
	pv->set						(coords.lt.x,	coords.rb.y,	C, tex_coords.lt.x,	tex_coords.rb.y);	pv++;
	pv->set						(coords.lt.x,	coords.lt.y,	C, tex_coords.lt.x,	tex_coords.lt.y);	pv++;
	pv->set						(coords.rb.x,	coords.rb.y,	C, tex_coords.rb.x,	tex_coords.rb.y);	pv++;
	pv->set						(coords.rb.x,	coords.lt.y,	C, tex_coords.rb.x,	tex_coords.lt.y);	pv++;
	RCache.Vertex.Unlock		(4,ll_hGeom.stride());

	RCache.set_Shader			(sh);
	RCache.set_Geometry			(ll_hGeom);
	RCache.Render				(D3DPT_TRIANGLELIST,Offset,0,4,0,2);
}

u32 calc_progress_color(u32 idx, u32 total, int stage, int max_stage)
{
	float kk			= (float(stage+1)/float(max_stage))*(total);
	float f				= 1/(exp((float(idx)-kk)*0.5f)+1.0f);

	return XrColor::color_argb_f		(f,1.0f,1.0f,1.0f);
}

#define IsSpace(ch)       ((ch) == ' ' || (ch) == '\t' || (ch) == '\r' || (ch) == '\n' || (ch) == ',' || (ch) == '.' || (ch) == ':' || (ch) == '!')

void parse_word(LPCSTR str, CGameFont* font, float& length, LPCSTR& next_word)
{
	length					= 0.0f;
	while(*str && !IsSpace(*str))
	{
//		length  += font->GetCharTC(*str).z;
		length  += font->SizeOf_(*str);
		++str;
	}
	next_word = (*str) ? str+1 : str;
}

void draw_multiline_text(CGameFont* F, float fTargetWidth, LPCSTR pszText)
{
	if(!pszText || xr_strlen(pszText)==0)
		return;

	LPCSTR ch				= pszText;
	float curr_word_len		= 0.0f;
	LPCSTR next_word		= NULL;

	float curr_len			= 0.0f;
	string512				buff;
	buff[0]					= 0;
	while(*ch)
	{
		parse_word			(ch, F, curr_word_len, next_word);
		if(curr_len+curr_word_len > fTargetWidth)
		{
			F->OutNext		(buff);
			curr_len		= 0.0f;
			buff[0]			= 0;
		}else
		{
			curr_len		+= curr_word_len;
			strncpy_s		(buff+xr_strlen(buff), sizeof(buff)-xr_strlen(buff), ch, next_word-ch);
			ch				= next_word;
		}
		if(0==*next_word) //end of text
		{
			strncpy_s		(buff+xr_strlen(buff), sizeof(buff)-xr_strlen(buff), ch, next_word-ch);
			F->OutNext		(buff);
			break;
		}
	}
}

void dxApplicationRenderSOC::Copy(IApplicationRender & _in)
{
	*this = *(dxApplicationRenderSOC*)&_in;
}
void dxApplicationRenderSOC::LoadTitleInt(LPCSTR str)
{
	load_stage++;

	VERIFY(str && xr_strlen(str) < 256);
	strcpy_s(app_title, str);
	Log(app_title);
	if (g_pGamePersistent->GameType() == 1)
		max_load_stage = 17;
	else
		max_load_stage = 14;
}
LPCSTR _GetFontTexName(LPCSTR section)
{
	static char* tex_names[] = { "texture800","texture","texture1600" };
	int def_idx = 1;//default 1024x768
	int idx = def_idx;

#if 0
	u32 w = Device.dwWidth;

	if (w <= 800)		idx = 0;
	else if (w <= 1280)idx = 1;
	else 			idx = 2;
#else
	u32 h = Device.dwHeight;

	if (h <= 600)		idx = 0;
	else if (h <= 900)	idx = 1;
	else 			idx = 2;
#endif


	while (idx >= 0) {
		if (pSettings->line_exist(section, tex_names[idx]))
			return pSettings->r_string(section, tex_names[idx]);
		--idx;
	}
	return pSettings->r_string(section, tex_names[def_idx]);
}

void _InitializeFont(CGameFont*& F, LPCSTR section, u32 flags)
{
	LPCSTR font_tex_name = _GetFontTexName(section);
	R_ASSERT(font_tex_name);

	if (!F) {
		F = xr_new<CGameFont>("font", font_tex_name, flags);
		//Device.seqRender.Add(F, REG_PRIORITY_LOW - 1000);
	}
	else
		F->Initialize("font", font_tex_name);

	if (pSettings->line_exist(section, "size")) {
		float sz = pSettings->r_float(section, "size");
		if (flags&CGameFont::fsDeviceIndependent)	F->SetHeightI(sz);
		else										F->SetHeight(sz);
	}
	if (pSettings->line_exist(section, "interval"))
		F->SetInterval(pSettings->r_fvector2(section, "interval"));

}

void dxApplicationRenderSOC::LoadBegin()
{

		_InitializeFont(pFontSystem, "ui_font_graffiti19_russian", 0);

		ll_hGeom.create(FVF::F_TL, RCache.Vertex.Buffer(), RCache.QuadIB);
		sh_progress.create("hud\\default", "ui\\ui_load");
		ll_hGeom2.create(FVF::F_TL, RCache.Vertex.Buffer(), NULL);
		load_stage = 0;

		if (g_pGamePersistent->GameType() == 1)
			max_load_stage = 17;
		else
			max_load_stage = 14;

}

void dxApplicationRenderSOC::destroy_loading_shaders()
{
	if (pFontSystem)xr_delete(pFontSystem); pFontSystem = 0;
	hLevelLogo.destroy();
	sh_progress.destroy();
}

void dxApplicationRenderSOC::setLevelLogo(LPCSTR pszLogoName)
{
	if (FS.ExistFile(TEXT("%textures%"), pszLogoName,".dds"))
	{
		hLevelLogo.create("font", pszLogoName);
	}
	else
	{
		hLevelLogo.create("font", "intro\\intro_no_start_picture");
	}
}
u32 calc_progress_color_soc(u32 idx, u32 total, int stage, int max_stage)
{
	if (idx > (total / 2))
		idx = total - idx;


	float kk = (float(stage + 1) / float(max_stage))*(total / 2.0f);
	float f = 1 / (exp((float(idx) - kk)*0.5f) + 1.0f);

	return XrColor::color_argb_f(f, 1.0f, 1.0f, 1.0f);
}

void dxApplicationRenderSOC::load_draw_internal(CApplication & owner)
{
	if (!sh_progress) {
#if (RENDER==R_R2)|| (RENDER==R_R1)
		CHK_DX(HW.pDevice->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1, 0));
#else
		//HW.pDevice->ClearRenderTargetView(HW.Rend)
#endif
		return;
	}
	// Draw logo
	u32	Offset;
	u32	C = 0xffffffff;
	u32	_w = Device.dwWidth;
	u32	_h = Device.dwHeight;
	FVF::TL* pv = NULL;

	//progress
	float bw = 1024.0f;
	float bh = 768.0f;
	Fvector2					k; k.set(float(_w) / bw, float(_h) / bh);

	RCache.set_Shader(sh_progress);
	CTexture*	T = RCache.get_ActiveTexture(0);
	Fvector2					tsz;
	tsz.set((float)T->get_Width(), (float)T->get_Height());
	Frect						back_text_coords;
	Frect						back_coords;
	Fvector2					back_size;

	//progress background
	static float offs = -0.5f;

	back_size.set(1024, 768);
	back_text_coords.lt.set(0, 0); back_text_coords.rb.add(back_text_coords.lt, back_size);
	back_coords.lt.set(offs, offs); back_coords.rb.add(back_coords.lt, back_size);

	back_coords.lt.mul(k); back_coords.rb.mul(k);

	back_text_coords.lt.x /= tsz.x; back_text_coords.lt.y /= tsz.y; back_text_coords.rb.x /= tsz.x; back_text_coords.rb.y /= tsz.y;
	pv = (FVF::TL*) RCache.Vertex.Lock(4, ll_hGeom.stride(), Offset);
	pv->set(back_coords.lt.x, back_coords.rb.y, C, back_text_coords.lt.x, back_text_coords.rb.y);	pv++;
	pv->set(back_coords.lt.x, back_coords.lt.y, C, back_text_coords.lt.x, back_text_coords.lt.y);	pv++;
	pv->set(back_coords.rb.x, back_coords.rb.y, C, back_text_coords.rb.x, back_text_coords.rb.y);	pv++;
	pv->set(back_coords.rb.x, back_coords.lt.y, C, back_text_coords.rb.x, back_text_coords.lt.y);	pv++;
	RCache.Vertex.Unlock(4, ll_hGeom.stride());

	RCache.set_Geometry(ll_hGeom);
	RCache.Render(D3DPT_TRIANGLELIST, Offset, 0, 4, 0, 2);

	//progress bar
	back_size.set(268, 37);
	back_text_coords.lt.set(0, 768); back_text_coords.rb.add(back_text_coords.lt, back_size);
	back_coords.lt.set(379, 726); back_coords.rb.add(back_coords.lt, back_size);

	back_coords.lt.mul(k); back_coords.rb.mul(k);

	back_text_coords.lt.x /= tsz.x; back_text_coords.lt.y /= tsz.y; back_text_coords.rb.x /= tsz.x; back_text_coords.rb.y /= tsz.y;



	u32 v_cnt = 40;
	pv = (FVF::TL*)RCache.Vertex.Lock(2 * (v_cnt + 1), ll_hGeom2.stride(), Offset);
	FVF::TL* _pv = pv;
	float pos_delta = back_coords.width() / v_cnt;
	float tc_delta = back_text_coords.width() / v_cnt;
	u32 clr = C;

	for (u32 idx = 0; idx < v_cnt + 1; ++idx) {
		clr = calc_progress_color_soc(idx, v_cnt, load_stage, max_load_stage);
		pv->set(back_coords.lt.x + pos_delta * idx + offs, back_coords.rb.y + offs, 0 + XrMath::EPS_S, 1, clr, back_text_coords.lt.x + tc_delta * idx, back_text_coords.rb.y);	pv++;
		pv->set(back_coords.lt.x + pos_delta * idx + offs, back_coords.lt.y + offs, 0 + XrMath::EPS_S, 1, clr, back_text_coords.lt.x + tc_delta * idx, back_text_coords.lt.y);	pv++;
	}
	VERIFY(u32(pv - _pv) == 2 * (v_cnt + 1));
	RCache.Vertex.Unlock(2 * (v_cnt + 1), ll_hGeom2.stride());

	RCache.set_Geometry(ll_hGeom2);
	RCache.Render(D3DPT_TRIANGLESTRIP, Offset, 2 * v_cnt);


	// Draw title
	VERIFY(pFontSystem);
	pFontSystem->Clear();
	pFontSystem->SetColor(XrColor::color_rgba(157, 140, 120, 255));
	pFontSystem->SetAligment(CGameFont::alCenter);
	pFontSystem->OutI(0.f, 0.815f, app_title);
	pFontSystem->OnRender();


	//draw level-specific screenshot
	if (hLevelLogo) {
		Frect						r;
		r.lt.set(257, 369);
		r.lt.x += offs;
		r.lt.y += offs;
		r.rb.add(r.lt, Fvector2().set(512, 256));
		r.lt.mul(k);
		r.rb.mul(k);
		pv = (FVF::TL*) RCache.Vertex.Lock(4, ll_hGeom.stride(), Offset);
		pv->set(r.lt.x, r.rb.y, C, 0, 1);	pv++;
		pv->set(r.lt.x, r.lt.y, C, 0, 0);	pv++;
		pv->set(r.rb.x, r.rb.y, C, 1, 1);	pv++;
		pv->set(r.rb.x, r.lt.y, C, 1, 0);	pv++;
		RCache.Vertex.Unlock(4, ll_hGeom.stride());

		RCache.set_Shader(hLevelLogo);
		RCache.set_Geometry(ll_hGeom);
		RCache.Render(D3DPT_TRIANGLELIST, Offset, 0, 4, 0, 2);
	}
}

void dxApplicationRenderSOC::KillHW()
{
}
