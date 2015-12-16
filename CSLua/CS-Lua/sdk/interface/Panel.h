#pragma once
#include "InterfaceBase.h"

class CPanel : public InterfaceBase
{
public:
	const char* GetName(unsigned int Panel)
	{
		typedef const char*(__thiscall* fn)(void*, unsigned int);
		return VMT::getvfunc<fn>(this, 36)(this, Panel);
	}
};

extern CPanel* g_pPanel;