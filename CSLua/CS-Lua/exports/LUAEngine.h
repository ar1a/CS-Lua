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

	int GetLocalPlayer()
	{
		return m_pEngine->GetLocalPlayer();
	}

	void SetViewAngles(Vector angles)
	{
		m_pEngine->SetViewAngles(angles);
	}

	Vector GetViewAngles() const
	{
		Vector retn;
		m_pEngine->GetViewAngles(retn);
		return retn;
	}

	void Cmd(const char* str)
	{
		m_pEngine->ClientCmd(str);
	}


	matrix3x4_t GetMatrix()
	{
		return m_pEngine->WorldToScreenMatrix();
	}

private:
	CEngine* m_pEngine;

};
