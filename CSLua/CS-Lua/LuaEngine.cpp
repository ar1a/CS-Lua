#include "LuaEngine.h"

LuaEngine *g_pLuaEngine = new LuaEngine();

lua_State* LuaEngine::L()
{
	return m_L;
}

void LuaEngine::report_errors(int state)
{
	if (state)
	{
		std::cerr << "ERR: " << lua_tostring(m_L, -1) << std::endl;
		lua_pop(m_L, 1); //remove error
	}
}

void LuaEngine::ExecuteFile(const char* file)
{
	if (!file)
		return;

	int state = luaL_dofile(m_L, file);
	report_errors(state);
}


void LuaEngine::ExecuteString(const char* expression)
{
	if (!expression)
	{
		std::cerr << "ERR: null expression passed to script engine!" << std::endl;
		return;
	}

	int state = luaL_dostring(m_L, expression);
	report_errors(state);
}