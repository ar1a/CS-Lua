#include <iostream>
#include <Windows.h>
#include <lua.hpp>
#include <LuaBridge.h>
#include "LuaEngine.h"
#include "sdk\InterfaceManager.hpp"
#include "exports\Exports.h"
#include "sdk\cmove.h"
#include "sdk\InputManager.h"

LUAInterfaces g_Interfaces;
LUAUtils g_Utils;

typedef void(__cdecl *MsgFn)(char const* pMsg, va_list);
void Msg(char const* msg)
{
	if (!msg)
		return;
	static MsgFn oMsg = (MsgFn)GetProcAddress(GetModuleHandle("tier0.dll"), "Msg");
	char buffer[989];
	va_list list;
	va_start(list, msg);
	vsprintf(buffer, msg, list);
	va_end(list);
	oMsg(buffer, list);
}


void RegEverything(lua_State* L)
{
	using namespace luabridge;
	getGlobalNamespace(L)
		.beginNamespace("Game")
			.addFunction("Msg", &Msg)			
			.addVariable("Interfaces", &g_Interfaces, false)
			.addVariable("Utils", &g_Utils, false)
			.beginClass<LUAInterfaces>("__Interfaces")
				.addFunction("GetEngine", &LUAInterfaces::GetEngine)
				.addFunction("GetEntityList", &LUAInterfaces::GetEntityList)
				.addFunction("GetTrace", &LUAInterfaces::GetTrace)
			.endClass()
			.beginClass<LUAEngine>("EngineInterface")
				.addFunction("GetScreenSize", &LUAEngine::GetScreenSize)
				.addFunction("GetLocalPlayer", &LUAEngine::GetLocalPlayer)
				.addProperty("viewangles", &LUAEngine::GetViewAngles, &LUAEngine::SetViewAngles)
				.addFunction("Cmd", &LUAEngine::Cmd)
			.endClass()
			.beginClass<Vector>("Vector")
				.addConstructor<void(*)()>()
				.addConstructor<void(*)(float, float, float)>()
				.addFunction("__eq", &Vector::operator==)
				.addFunction("__add", &Vector::operator+)
				.addFunction("__sub", &Vector::sub)
				.addFunction("__mul", &Vector::mul)
				.addFunction("__div", &Vector::div)
				.addData("x", &Vector::x)
				.addData("y", &Vector::y)
				.addData("z", &Vector::z)
				.addFunction("Length", &Vector::Length)
				.addFunction("Angle", &Vector::Angle)
				.addFunction("Distance", &Vector::DistTo)
			.endClass()
			.beginClass<Vec2>("Vec2")
				.addConstructor<void(*)()>()
				.addConstructor<void(*)(float, float)>()
				.addData("x", &Vec2::x)
				.addData("y", &Vec2::y)
			.endClass()
			.beginClass<LuaUserCmd>("UserCmd")
				.addProperty("buttons", &LuaUserCmd::GetButtons, &LuaUserCmd::SetButtons)
				.addProperty("viewangles", &LuaUserCmd::GetViewAngles, &LuaUserCmd::SetViewAngles)
				.addFunction("KeyDown", &LuaUserCmd::KeyDown)
				.addFunction("AddButton", &LuaUserCmd::AddButton)
				.addFunction("RemoveButton", &LuaUserCmd::RemoveButton)
			.endClass()
		.beginClass<LUAEntity>("Entity")
			.addFunction("IsValid", &LUAEntity::IsValid)
			.addFunction("GetPos", &LUAEntity::GetPos)
			.addFunction("GetHealth", &LUAEntity::GetHealth)
			.addFunction("GetFlags", &LUAEntity::GetFlags)
			.addFunction("GetShootPos", &LUAEntity::GetShootPos)
			.addFunction("IsDormant", &LUAEntity::IsDormant)
			.addFunction("IsAlive", &LUAEntity::IsAlive)
			.addFunction("GetTeam", &LUAEntity::GetTeam)
			.addFunction("GetPunch", &LUAEntity::GetPunch)
			.addFunction("IsReal", &LUAEntity::IsReal)
		.endClass()
		.beginClass<LUAEntityList>("EntityList")
			.addFunction("GetEntity", &LUAEntityList::GetEntity)
			.addFunction("GetHighestEntityIndex", &LUAEntityList::GetHighestEntityIndex)
		.endClass()
		.beginClass<LUATrace>("EngineTrace")
			.addFunction("Trace", &LUATrace::TraceRay)
		.endClass()
		.beginClass<LUAtrace_t>("trace_t")
			.addFunction("DidHit", &LUAtrace_t::DidHit)
			.addFunction("DidHitEntity", &LUAtrace_t::DidHitEntity)
			.addFunction("GetHitbox", &LUAtrace_t::GetHitbox)
			.addFunction("IsVisible", &LUAtrace_t::IsVisible)
			.addFunction("GetEndPos", &LUAtrace_t::GetEndPos)
			.addFunction("GetEntity", &LUAtrace_t::GetEntity)
		.endClass()
		.beginClass<LUAUtils>("__Utils")
			.addFunction("WorldToScreen", &LUAUtils::WorldToScreen)
			.addFunction("IsPlayer", &LUAUtils::IsPlayer)
			.addFunction("GetHitboxPos", &LUAUtils::GetHitboxPosition)
		.endClass()
		.beginClass<KeyData>("KeyData")
			.addData("key", &KeyData::key, false)
			.addProperty("down", &KeyData::IsDown)
		.endClass()
		.endNamespace();

	g_pLuaEngine->ExecuteString("bit = bit32");
}

bool Keyboard(const KeyData &data)
{
	using namespace luabridge;
	LuaRef hook = getGlobal(g_pLuaEngine->L(), "hook");
	if (hook["Call"].isFunction())
		hook["Call"]("Key", data);
	else
		printf("ERR: hook.Call not found!\n");
}

void StartThread()
{
	inputmanager::Init();

	InterfaceManager::GetInterfaces();
	g_pEngine->ClientCmd("clear");

	VMT* client = new VMT(g_pClientMode);
	client->init();
	client->setTableHook();
	client->hookFunction(24, hkCreateMove);


	RegEverything(g_pLuaEngine->L());
	Msg("\n\n\n    _____  _____  _                        ___  \n \
  / ____|/ ____|| |                      |__ \\ \n \
 | |    | (___(_) |    _   _  __ _  __   __ ) |\n \
 | |     \\___ \\ | |   | | | |/ _` | \\ \\ / // / \n \
 | |____ ____) || |___| |_| | (_| |  \\ V // /_ \n \
  \\_____|_____(_)______\\__,_|\\__,_|   \\_/|____|\n\n\n");
	Msg("CS:Lua v2 loaded! Enjoy!\n");
	while (1)
	{
		if (GetAsyncKeyState(VK_HOME) & 0x8000)
		{
			g_pLuaEngine->ExecuteFile("hook.lua");
			Msg("---------------------\nhook.lua loaded\n---------------------\n");
			g_pLuaEngine->ExecuteFile("exec.lua");
			Sleep(300);
		}
		Sleep(1);
	}
}

void InitConsole(char* name)
{
	AllocConsole();

	HWND hwnd = GetConsoleWindow();

	HMENU hMenu = GetSystemMenu(hwnd, FALSE);
	if (hMenu) DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);

	SetConsoleTitle(name);
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
}
BOOL WINAPI DllMain(HINSTANCE module_handle, DWORD reason_for_call, LPVOID reserved)
{
	if (reason_for_call == DLL_PROCESS_ATTACH)
	{
		InitConsole("LUA");
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)StartThread, 0, 0, 0);
	}
	return TRUE;
}