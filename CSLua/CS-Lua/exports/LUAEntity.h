#pragma once
#include "BaseLuaExport.h"
#include "../sdk/CBaseEntity.h"

class LUAEntity : public BaseLuaExport
{
public:
	LUAEntity(CBaseEntity* ent) : m_pEnt(ent) {}

	bool IsValid()
	{
		return m_pEnt;
	}

	Vector GetPos()
	{
		return m_pEnt->GetAbsOrigin();
	}

	int GetHealth()
	{
		return m_pEnt->GetHealth();
	}

	unsigned int GetFlags()
	{
		return m_pEnt->GetFlags();
	}

	Vector GetShootPos()
	{
		return m_pEnt->GetEyePosition();
	}

	bool IsDormant()
	{
		return m_pEnt->IsDormant();
	}

	bool IsAlive()
	{
		return m_pEnt->IsAlive();
	}

	int GetTeam()
	{
		return m_pEnt->GetTeamNum();
	}

	Vector GetPunch()
	{
		return m_pEnt->GetPunch();
	}

	bool IsReal()
	{
		return (IsValid() && IsAlive() && !IsDormant());
	}

	operator CBaseEntity*()
	{
		return m_pEnt;
	}

	int GetAmmo()
	{
		CBaseEntity* weapon = g_pClientEntList->GetClientEntityFromHandle(m_pEnt->GetWeapon());
		if (!weapon) return -1;
		return weapon->GetClip();
	}
	int GetBackupAmmo()
	{
		CBaseEntity* weapon = g_pClientEntList->GetClientEntityFromHandle(m_pEnt->GetWeapon());
		if (!weapon) return -1;
		return weapon->GetClipReserve();
	}

	int GetArmor()
	{
		return m_pEnt->GetArmor();
	}

	int GetIndex()
	{
		return m_pEnt->GetIndex();
	}
private:
	CBaseEntity* m_pEnt;
};
