#pragma once

#include "Vector.h"
#include "math.h"
#include "VMT.h"
#define	FL_ONGROUND					(1 << 0)	
#define FL_DUCKING					(1 << 1)	
#define	FL_WATERJUMP				(1 << 2)	
#define FL_ONTRAIN					(1 << 3)
#define FL_INRAIN					(1 << 4)	
#define FL_FROZEN					(1 << 5)
#define FL_ATCONTROLS				(1 << 6)
#define	FL_CLIENT					(1 << 7)	
#define FL_FAKECLIENT				(1 << 8)	
#define	FL_INWATER					(1 << 9)										
#define	FL_FLY						(1 << 10)	
#define	FL_SWIM						(1 << 11)
#define	FL_CONVEYOR					(1 << 12)
#define	FL_NPC						(1 << 13)
#define	FL_GODMODE					(1 << 14)
#define	FL_NOTARGET					(1 << 15)
#define	FL_AIMTARGET				(1 << 16)	
#define	FL_PARTIALGROUND			(1 << 17)	
#define FL_STATICPROP				(1 << 18)
#define FL_GRAPHED					(1 << 19) 
#define FL_GRENADE					(1 << 20)
#define FL_STEPMOVEMENT				(1 << 21)	
#define FL_DONTTOUCH				(1 << 22)
#define FL_BASEVELOCITY				(1 << 23)
#define FL_WORLDBRUSH				(1 << 24)	
#define FL_OBJECT					(1 << 25) 
#define FL_KILLME					(1 << 26)
#define FL_ONFIRE					(1 << 27)	
#define FL_DISSOLVING				(1 << 28)
#define FL_TRANSRAGDOLL				(1 << 29)
#define FL_UNBLOCKABLE_BY_PLAYER	(1 << 30) 
#define GETNETVAR(retntype, funcname, offset) \
	retntype funcname() { \
		return *(retntype*)(this + offset); }
struct ClientClass;

class CBaseEntity
{
public:

	GETNETVAR(int,  GetTeamNum,0xF0);
	GETNETVAR(int,  GetHealth, 0xFC);
	GETNETVAR(unsigned int, GetFlags, 0x100);
	GETNETVAR(int,  GetHitboxSet, 0x9F4);
	GETNETVAR(Vector, GetPunch, 0x2FF8);
	GETNETVAR(int, GetTickBase, 0x33EC);
	GETNETVAR(HANDLE, GetWeapon, 0x2EC8);
	GETNETVAR(int, GetClip, 0x31D4); //GUN ONLY
	GETNETVAR(int, GetClipReserve, 0x31DC);
	GETNETVAR(int, GetArmor, 0xA8C4);

	void SetFlags(unsigned int flag)
	{
		*(unsigned int*)(this + 0x100) = flag;
	}

	Vector GetEyePosition()
	{
		static int offset = 0x104;
		Vector vecViewOffset = *(Vector*)(this + offset);
		return GetAbsOrigin() + vecViewOffset;
	}

	Vector& GetAbsOrigin()
	{
		typedef Vector&(__thiscall* fn)(void*);
		return VMT::getvfunc<fn>(this, 10)(this);
	}

	ClientClass* GetClientClass()
	{
		void* networkable = (void*)(this + 0x8);
		typedef ClientClass*(__thiscall* fn)(void*);
		return VMT::getvfunc<fn>(networkable, 2)(networkable);
	}

	bool IsDormant()
	{
		void* networkable = (void*)(this + 0x8);
		typedef bool(__thiscall* fn)(void*);
		return VMT::getvfunc<fn>(networkable, 9)(networkable);
	}

	GETNETVAR(unsigned char, GetLifeState, 0x25B);


	bool IsAlive()
	{
		return (GetLifeState() == 0);
	}

	int GetIndex()
	{
		return *(int*)(this + 0x64);
	}

	bool SetupBones(matrix3x4_t* pBoneToWorldOut, int maxBones, int boneMask, float currentTime)
	{
		void* renderable = (void*)(this + 0x4);
		typedef bool(__thiscall* fn)(void*, matrix3x4_t*, int, int, float);
		return VMT::getvfunc<fn>(renderable, 13)(renderable, pBoneToWorldOut, maxBones, boneMask, currentTime);
	}


	// model_t
	void* GetModel()
	{
		void* renderable = (void*)(this + 0x4);
		typedef void*(__thiscall* fn)(void*);
		return VMT::getvfunc<fn>(renderable, 8)(renderable);
	}



};