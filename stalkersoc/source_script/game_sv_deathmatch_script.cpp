#include "pch_script.h"
#include "game_sv_deathmatch.h"
#include "xrServer_script_macroses.h"
#include "xrserver.h"


/*
#pragma optimize("s",on)
void game_sv_Deathmatch::script_register(lua_State *L)
{
	typedef CWrapperBase<game_sv_Deathmatch> WrapType;
	module(L)
		[
			luabind::class_< game_sv_Deathmatch, WrapType, game_sv_GameState >("game_sv_Deathmatch")
			.def(	constructor<>())
			.def("GetTeamData",			&game_sv_Deathmatch::GetTeamData)
			
			
			.def("type_name",		&WrapType::type_name, &WrapType::type_name_static)
//			.def("Money_SetStart",		&CWrapperBase<game_sv_Deathmatch>::Money_SetStart, &CWrapperBase<game_sv_Deathmatch>::Money_SetStart_static)
		];

}
*/