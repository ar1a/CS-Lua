#pragma once
#include "Vector.h"
#include "usercmd.h"
#include "CBaseEntity.h"
#include "interface\EntityList.h"
#include "interface\Engine.h"
#include "../exports\Exports.h"
#include <LuaBridge.h>
#include "../LuaEngine.h"
inline void ClampAngle(Vector& qaAng)
{
	if (qaAng[0] > 89.0f)
		qaAng[0] = 89.0f;

	if (qaAng[0] < -89.0f)
		qaAng[0] = -89.0f;

	while (qaAng[1] > 180)
		qaAng[1] -= 360;

	while (qaAng[1] < -180)
		qaAng[1] += 360;

	qaAng.z = 0;
}


bool __stdcall hkCreateMove(float frametime, CUserCmd* cmd)
{
	LOCKLUA();
	if (!g_pLuaEngine->L())
		return false;


	using namespace luabridge;

	LuaRef hook = getGlobal(g_pLuaEngine->L(), "hook");
	if (hook["Call"].isFunction())
	{
		try {
			hook["Call"]("CreateMove", LuaUserCmd(cmd), frametime);
		}
		catch (LuaException const& e)
		{
			//If this is called then there was no hook for "CreateMove" registered. This isn't an issue.
		}
	}
	else
	{
		printf("ERR: hook.Call not found!\n");
	}



	ClampAngle(cmd->viewangles);



	return false;
}