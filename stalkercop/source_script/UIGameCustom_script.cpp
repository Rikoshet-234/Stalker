#include "pch_script.h"
#include "UIGameCustom.h"
#include "level.h"
#include "UI/uistatic.h"

using namespace luabind;

CUIGameCustom* get_hud(){
	return CurrentGameUI();
}

#pragma optimize("s",on)
void CUIGameCustom::script_register(lua_State *L)
{
	module(L)
		[
			class_< StaticDrawableWrapper >("StaticDrawableWrapper")
			.def_readwrite("m_endTime",		&StaticDrawableWrapper::m_endTime)
			.def("wnd",					&StaticDrawableWrapper::wnd),

			class_< CUIGameCustom >("CUIGameCustom")
			.def("AddDialogToRender",		&CUIGameCustom::AddDialogToRender)
			.def("RemoveDialogToRender",	&CUIGameCustom::RemoveDialogToRender)
			.def("AddCustomStatic",			&CUIGameCustom::AddCustomStatic)
			.def("RemoveCustomStatic",		&CUIGameCustom::RemoveCustomStatic)
			.def("HideActorMenu",			&CUIGameCustom::HideActorMenu)
			.def("HidePdaMenu",				&CUIGameCustom::HidePdaMenu)
			.def("show_messages",			&CUIGameCustom::ShowMessagesWindow)
			.def("hide_messages",			&CUIGameCustom::HideMessagesWindow)
			.def("GetCustomStatic",			&CUIGameCustom::GetCustomStatic)
			.def("update_fake_indicators",	&CUIGameCustom::update_fake_indicators)
			.def("enable_fake_indicators",	&CUIGameCustom::enable_fake_indicators),
			def("get_hud",					&get_hud)
		];
}
