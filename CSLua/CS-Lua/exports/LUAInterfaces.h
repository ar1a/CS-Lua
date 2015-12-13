#pragma once
#include "BaseLuaExport.h"
#include "../sdk/interface/Engine.h"
#include "../sdk/interface/EntityList.h"
#include "LUAEntityList.h"

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
};
