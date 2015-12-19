#pragma once
#include <iostream>
#include <lua.hpp>
#include <mutex>

#define LOCKLUA() std::lock_guard<std::mutex> lock(g_pLuaEngine->m)

class LuaEngine
{
public:
	LuaEngine() : m_L(luaL_newstate()) { luaL_openlibs(m_L); }
	LuaEngine(const LuaEngine& other);
	LuaEngine& operator=(const LuaEngine&);
	~LuaEngine() { lua_close(m_L); }

	lua_State* L();
	
	void ExecuteFile(const char* file);

	void ExecuteString(const char* expression);

	void Reset()
	{
		if(m_L)
			lua_close(m_L);
		m_L = luaL_newstate();
		luaL_openlibs(m_L);
	}

	std::mutex m;
private:
	lua_State* m_L;
	void report_errors(int state);
};
extern LuaEngine *g_pLuaEngine;