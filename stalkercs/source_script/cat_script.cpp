#include "pch_script.h"
#include "ai/monsters/cat/cat.h"

using namespace luabind;

#pragma optimize("s",on)
void CCat::script_register(lua_State *L)
{
	module(L)
	[
		class_<CCat,CGameObject>("CCat")
			.def(constructor<>())
	];
}
