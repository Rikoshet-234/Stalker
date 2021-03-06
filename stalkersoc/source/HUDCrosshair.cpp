// HUDCrosshair.cpp:  ������� �������, ������������ ������� ���������
// 
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XrRender/UIShader.h"
#include "XrRender/UIRender.h"
#include "HUDCrosshair.h"
#include "UIStaticItem.h"

CHUDCrosshair::CHUDCrosshair	()
{
//.	hGeomLine.create			(FVF::F_TL0uv,RCache.Vertex.Buffer(),0);
//.	hShader.create				("editor\\wire");
	hShader->create("hud\\crosshair");

	//��������� � ��������� ����� ������
//	center.set(int(Device.dwWidth)/2,int(Device.dwHeight)/2);
	radius = 0;
}


CHUDCrosshair::~CHUDCrosshair	()
{
	/*hGeomLine.destroy			();
	hShader.destroy				();*/

}

void CHUDCrosshair::Load		()
{
	//��� ������� � ��������� �� ����� ������
	//����� �������� 
	cross_length_perc = pSettings->r_float (HUD_CURSOR_SECTION, "cross_length");
//	cross_length = XrMath::iFloor(0.5f + cross_length_perc*float(Device.dwWidth));

	min_radius_perc = pSettings->r_float (HUD_CURSOR_SECTION, "min_radius");
	//min_radius = XrMath::iFloor(0.5f + min_radius_perc*float(Device.dwWidth));

	max_radius_perc = pSettings->r_float (HUD_CURSOR_SECTION, "max_radius");
	//max_radius = XrMath::iFloor(0.5f + max_radius_perc*float(Device.dwWidth));

	cross_color = pSettings->r_fcolor (HUD_CURSOR_SECTION, "cross_color").get();


	radius_speed_perc = pSettings->r_float (HUD_CURSOR_SECTION, "radius_lerp_speed");
}

//���������� radius �� min_radius �� max_radius
void CHUDCrosshair::SetDispersion	(float disp)
{ 
	Fvector4 r;
	Fvector R			= { VIEWPORT_NEAR*XrMath::sin(disp), 0.f, VIEWPORT_NEAR };
	Device.mProject.transform	(r,R);

	Fvector2		scr_size;
	scr_size.set	(float(::Render->getTarget()->get_width()), float(::Render->getTarget()->get_height()));
	float radius_pixels		= XrMath::abs(r.x)*scr_size.x/2.0f;
	//	XrMath::clamp(radius_pixels, min_radius, max_radius);
	target_radius		= radius_pixels; 
}

extern ENGINE_API BOOL g_bRendering; 
void CHUDCrosshair::OnRender ()
{
	VERIFY			(g_bRendering);
	Fvector2		center;
	Fvector2		scr_size;
	scr_size.set	(float(::Render->getTarget()->get_width()), float(::Render->getTarget()->get_height()));
	center.set		(scr_size.x/2.0f, scr_size.y/2.0f);

	// draw back
	UIRender->StartPrimitive(10, IUIRender::ptLineList,IUIRender::pttTL);


	float cross_length = cross_length_perc*scr_size.x;
	float min_radius = min_radius_perc*scr_size.x;
	float max_radius = max_radius_perc*scr_size.x;

	XrMath::clamp(target_radius, min_radius, max_radius);

	float x_min = min_radius + radius;
	float x_max = x_min + cross_length;

	float y_min = x_min;
	float y_max = x_max;

	// 0
	UIRender->PushPoint(center.x, center.y + y_min, 0, cross_color, 0, 0);
	UIRender->PushPoint(center.x, center.y + y_max, 0, cross_color, 0, 0);
	// 1
	UIRender->PushPoint(center.x, center.y - y_min, 0, cross_color, 0, 0);
	UIRender->PushPoint(center.x, center.y - y_max, 0, cross_color, 0, 0);
	// 2
	UIRender->PushPoint(center.x + x_min, center.y, 0, cross_color, 0, 0);
	UIRender->PushPoint(center.x + x_max, center.y, 0, cross_color, 0, 0);
	// 3
	UIRender->PushPoint(center.x - x_min, center.y, 0, cross_color, 0, 0);
	UIRender->PushPoint(center.x - x_max, center.y, 0, cross_color, 0, 0);

	// point
	UIRender->PushPoint(center.x - 0.5f, center.y, 0, cross_color, 0, 0);
	UIRender->PushPoint(center.x + 0.5f, center.y, 0, cross_color, 0, 0);


	// render	
	UIRender->SetShader(*hShader);
	UIRender->FlushPrimitive();


	if(!XrMath::fsimilar(target_radius,radius))
	{
		float sp				= radius_speed_perc * scr_size.x ;
		float radius_change		= sp*Device.fTimeDelta;
		XrMath::clamp					(radius_change, 0.0f, sp*0.033f); // XrMath::clamp to 30 fps
		XrMath::clamp					(radius_change, 0.0f, XrMath::abs(target_radius-radius));

		if(target_radius < radius)
			radius -= radius_change;
		else
			radius += radius_change;
	};
}