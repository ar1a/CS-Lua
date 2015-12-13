#pragma once
#include "InterfaceBase.h"
#include "../usercmd.h"
#include "../CBaseEntity.h"
//VClientPrediction client.dll
class CPrediction : public InterfaceBase
{
public:
	void SetupMove(CBaseEntity* ent, CUserCmd* cmd, void* move, void* movedata)
	{
		typedef void(__thiscall* fn)(void*, CBaseEntity*, CUserCmd*, void*, void*);
		VMT::getvfunc<fn>(this, 20)(this, ent, cmd, move, movedata);
	}

	void FinishMove(CBaseEntity* ent, CUserCmd* cmd, void* movedata)
	{
		typedef void(__thiscall* fn)(void*, CBaseEntity*, CUserCmd*, void*);
		VMT::getvfunc<fn>(this, 21)(this, ent, cmd, movedata);
	}
};
extern CPrediction* g_pPrediction;

//GameMovement client.dll
class CMovement : public InterfaceBase
{
public:
	void ProcessMovement(CBaseEntity* ent, void* movedata)
	{
		typedef void(__thiscall* fn)(void*, CBaseEntity*, void*);
		VMT::getvfunc<fn>(this, 1)(this, ent, movedata);
	}

	void StartTrackPredictionErrors(CBaseEntity* ent)
	{
		typedef void(__thiscall* fn)(void*, CBaseEntity*);
		VMT::getvfunc<fn>(this, 3)(this, ent);
	}

	void FinishTrackPredictionErrors(CBaseEntity* ent)
	{
		typedef void(__thiscall* fn)(void*, CBaseEntity*);
		VMT::getvfunc<fn>(this, 4)(this, ent);
	}
};
extern CMovement* g_pMovement;