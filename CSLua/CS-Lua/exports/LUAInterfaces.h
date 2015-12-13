#pragma once
#include "BaseLuaExport.h"
#include "../sdk/interface/Engine.h"
#include "../sdk/interface/EngineTrace.h"
#include "../sdk/interface/EntityList.h"
#include "LUAEntityList.h"
#include "LUATrace.h"

class LUAInterfaces : public BaseLuaExport
{
public:
	LUAEngine GetEngine()
	{
		static LUAEngine engine(g_pEngine);
		return engine;
	}

	LUAEntityList GetEntityList()
	{
		static LUAEntityList list(g_pClientEntList);
		return list;
	}

	LUATrace GetTrace()
	{
		static LUATrace trace(g_pEngineTrace);
		return trace;
	}
};
