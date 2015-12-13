#pragma once
#include "BaseLuaExport.h"
#include "../sdk/usercmd.h"

class LuaUserCmd : public BaseLuaExport
{
public:
	LuaUserCmd(CUserCmd* cmd) : m_pCmd(cmd) {}

	operator CUserCmd*()
	{
		return m_pCmd;
	}

	unsigned int GetButtons() const
	{
		return m_pCmd->buttons;
	}

	Vector GetViewAngles() const
	{
		return m_pCmd->viewangles;
	}

	void SetButtons(unsigned int buttons)
	{
		m_pCmd->buttons = buttons;
	}

	void SetViewAngles(Vector& ang)
	{
		m_pCmd->viewangles = ang;
	}
	bool KeyDown(unsigned int key)
	{
		return m_pCmd->buttons & key;
	}
	void AddButton(unsigned int key)
	{
		m_pCmd->buttons |= key;
	}
	void RemoveButton(unsigned int key)
	{
		m_pCmd->buttons &= ~key;
	}

private:
	CUserCmd* m_pCmd;
};