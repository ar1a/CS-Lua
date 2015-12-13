#pragma once
#include "BaseLuaExport.h"
#include "../sdk/interface/Engine.h"
#include "../sdk/Vector.h"

class LUAEngine : public BaseLuaExport
{
public:
	LUAEngine(CEngine* engine) : m_pEngine(engine) {}
	Vec2 GetScreenSize()
	{
		int w, h;
		m_pEngine->GetScreenSize(w, h);
		return Vec2(w, h);
	}


private:
	CEngine* m_pEngine;

};
