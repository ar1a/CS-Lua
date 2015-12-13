#pragma once
#include "InterfaceBase.h"

class CBaseEntity;

class EntityList : public InterfaceBase
{
public:
	CBaseEntity* GetClientEntity(int index)
	{
		typedef CBaseEntity*(__thiscall* fn)(void*, int);
		return VMT::getvfunc<fn>(this, 3)(this, index);
	}

	CBaseEntity* GetClientEntityFromHandle(void* handle)
	{
		typedef CBaseEntity*(__thiscall* fn)(void*, void*);
		return VMT::getvfunc<fn>(this, 4)(this, handle);
	}

	int GetHighestEntityIndex()
	{
		typedef int(__thiscall* fn)(void*);
		return VMT::getvfunc<fn>(this, 6)(this);
	}

};
extern EntityList* g_pClientEntList;