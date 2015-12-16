#pragma warning(disable: 4099)
#pragma warning(disable: 4227)
#pragma warning(disable: 4244)
#pragma warning(disable: 4800)
#pragma warning(disable: 4101)
#include <iostream>
#include <Windows.h>
#include <lua.hpp>
#include <LuaBridge.h>
#include "LuaEngine.h"
#include "sdk\InterfaceManager.hpp"
#include "exports\Exports.h"
#include "sdk\cmove.h"
#include "sdk\InputManager.h"
#include "exports\LuaFiles.h"
#include "sdk\interface\Panel.h"
#include "Drawing.h"

LUAInterfaces g_Interfaces;
LUAUtils g_Utils;
CDrawing g_Drawing;

Vector Vector::ToScreen() {
	Vector screen(0, 0, 0);
	Vector point = *this;
	const VMatrix &worldToScreen = g_pEngine->WorldToScreenMatrix();
	screen.x = worldToScreen[0][0] * point[0] + worldToScreen[0][1] * point[1] + worldToScreen[0][2] * point[2] + worldToScreen[0][3];
	screen.y = worldToScreen[1][0] * point[0] + worldToScreen[1][1] * point[1] + worldToScreen[1][2] * point[2] + worldToScreen[1][3];
	float w = worldToScreen[3][0] * point[0] + worldToScreen[3][1] * point[1] + worldToScreen[3][2] * point[2] + worldToScreen[3][3];
	screen.z = 0.0f;

	if (w >= 0.001f)
	{
		int iWidth, iHeight;
		g_pEngine->GetScreenSize(iWidth, iHeight);

		screen.x *= 1.0f / w;
		screen.y *= 1.0f / w;

		float x = iWidth / 2;
		float y = iHeight / 2;
		x += 0.5 * screen.x * iWidth + 0.5;
		y -= 0.5 * screen.y * iHeight + 0.5;
		screen.x = x;
		screen.y = y;

		return screen;
	}
	screen.x *= 100000;
	screen.y *= 100000;

		return screen;
	
}
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
			.addVariable("Drawing", &g_Drawing, false)
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
				.addFunction("ToScreen", &Vector::ToScreen)
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
				.addFunction("IsPlayer", &LUAUtils::IsPlayer)
				.addFunction("GetHitboxPos", &LUAUtils::GetHitboxPosition)
			.endClass()
			.beginClass<KeyData>("KeyData")
				.addData("key", &KeyData::key, false)
				.addProperty("down", &KeyData::IsDown)
				.addProperty("held", &KeyData::IsHeld)
				.addData("processed", &KeyData::processed, false)
			.endClass()
			.beginClass<CDrawing>("__Drawing")
				.addFunction("DrawString", &CDrawing::DrawString)
				.addFunction("CreateFont", &CDrawing::CreateFont)
				.addFunction("DrawLine", &CDrawing::DrawLine)
				.addFunction("DrawFilledRect", &CDrawing::DrawFilledRect)
				.addFunction("DrawOutlinedRect", &CDrawing::DrawOutlinedRect)
				.addFunction("DrawRect", &CDrawing::DrawRect)
				.addFunction("SetDrawColor", &CDrawing::SetDrawColor)
			.endClass()
		.endNamespace();

	g_pLuaEngine->ExecuteString("bit = bit32");
}

bool Keyboard(const KeyData &data)
{
	std::unique_lock<std::mutex> lock(g_pLuaEngine->m);
	using namespace luabridge;
	LuaRef hook = getGlobal(g_pLuaEngine->L(), "hook");
	if (hook["Call"].isFunction())
	{
		try {
			hook["Call"]("Key", data);
		}
		catch (LuaException const& e)
		{
			//If this is called then there was no hook for "Key" registered. This isn't an issue.
		}
	}
	else
	{
		printf("ERR: KB -  hook.Call not found!\n");
	}

	lock.unlock();
	if (data.IsDown() && data.key == VK_DELETE) {
		lock.lock();
		g_pLuaEngine->ExecuteFile("exec.lua");
		lock.unlock();
	}

	return false;
}

typedef void(__thiscall* PaintTraverseFn)(void*,unsigned int, bool, bool);
PaintTraverseFn oPaintTraverse;
void __fastcall hkPaintTraverse(void* thisptr, void*, unsigned int a, bool b, bool c)
{
	oPaintTraverse(thisptr, a, b, c);
	static unsigned int mstp = 0;
	if (!mstp)
	{
		const char *pName = g_pPanel->GetName(a);
		if (strstr("FocusOverlayPanel", pName))
			mstp = a;
	}
	if (a == mstp)
	{
		LOCKLUA();
		if (!g_pLuaEngine->L())
		{
			return;
		}
		using namespace luabridge;
		LuaRef hook = getGlobal(g_pLuaEngine->L(), "hook");
		if (hook["Call"].isFunction())
		{
			try {
				hook["Call"]("Paint");
			}
			catch (LuaException const& e)
			{
				//If this is called then there was no hook for "Key" registered. This isn't an issue.
			}
		}
		else
		{
			printf("ERR: PT - hook.Call not found!\n");
		}

	}

}

void ResetEnvironment()
{
	g_pLuaEngine->Reset();
	g_pLuaEngine->ExecuteString(HOOK_CODE.c_str());
	Msg("---------------------\nhook.lua loaded\n---------------------\n");
	g_pLuaEngine->ExecuteFile("init.lua");
	Msg("---------------------\ninit.lua loaded\n---------------------\n");
	RegEverything(g_pLuaEngine->L());
}
void StartThread()
{
	inputmanager::Init();

	InterfaceManager::GetInterfaces();

	g_pEngine->ClientCmd("clear");

	LOCKLUA();

	VMT* client = new VMT(g_pClientMode);
	client->init();
	client->setTableHook();
	client->hookFunction(24, hkCreateMove);

	VMT* panel = new VMT(g_pPanel);
	panel->init();
	panel->setTableHook();
	oPaintTraverse = (PaintTraverseFn)panel->hookFunction(41, hkPaintTraverse);


	RegEverything(g_pLuaEngine->L());
	Msg("\n\n\n    _____  _____  _                        ___  \n \
  / ____|/ ____|| |                      |__ \\ \n \
 | |    | (___(_) |    _   _  __ _  __   __ ) |\n \
 | |     \\___ \\ | |   | | | |/ _` | \\ \\ / // / \n \
 | |____ ____) || |___| |_| | (_| |  \\ V // /_ \n \
  \\_____|_____(_)______\\__,_|\\__,_|   \\_/|____|\n\n\n");
	Msg("CS:Lua v2 loaded! Enjoy!\n");
	//g_pLuaEngine->ExecuteFile("hook.lua");
	g_pLuaEngine->ExecuteString(HOOK_CODE.c_str());
	Msg("---------------------\nhook.lua loaded\n---------------------\n");
	g_pLuaEngine->ExecuteFile("init.lua");
	Msg("---------------------\ninit.lua loaded\n---------------------\n");
	inputmanager::AddKeyboardCallback(Keyboard);
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