#pragma once
#include "BaseLuaExport.h"
#include "../sdk/interface/Engine.h"

class LUAInterfaces : public BaseLuaExport
{
public:
	LUAEngine GetEngine()
	{
		static LUAEngine engine(g_pEngine);
		return engine;
	}
};
