#include <iostream>
#include <Windows.h>
#include <lua.hpp>
#include <LuaBridge.h>
#include "LuaEngine.h"
#include "sdk\InterfaceManager.hpp"
#include "exports\Exports.h"

LUAInterfaces g_Interfaces;

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
			.beginClass<LUAInterfaces>("__Interfaces")
				.addFunction("GetEngine", &LUAInterfaces::GetEngine)
			.endClass()
			.beginClass<LUAEngine>("Engine")
				.addFunction("GetScreenSize", &LUAEngine::GetScreenSize)
			.endClass()
			.beginClass<Vector>("Vector")
				.addConstructor<void(*)()>()
				.addConstructor<void(*)(float, float, float)>()
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
		.endNamespace();
}

void StartThread()
{
	InterfaceManager::GetInterfaces();

	RegEverything(g_pLuaEngine->L());

	Msg("CS:Lua v2 loaded!\n");
	while (1)
	{
		if (GetAsyncKeyState(VK_HOME) & 0x8000)
		{
			g_pLuaEngine->ExecuteFile("hook.lua");
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