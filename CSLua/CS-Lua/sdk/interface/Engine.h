#pragma once
#include "InterfaceBase.h"
#include "../Vector.h"
#include "../math.h"
class player_info_t
{
public:
	char __pad1[0x10];
	char name[32];//80
	char __pad2[0x64];
	char guid[32 + 1];
	char __pad3[0x500];
};

struct VMatrix
{
public:
	float* operator[](int i) { return m[i]; }
	const float* operator[](int i) const { return m[i]; }
	float *Base() { return &m[0][0]; }
	const float *Base() const { return &m[0][0]; }

	float	m[4][4];
};
class CEngine : public InterfaceBase
{
public:
	void GetScreenSize(int& width, int& height)
	{
		typedef void(__thiscall* fn)(void*, int&, int&);
		VMT::getvfunc<fn>(this, 5)(this, width, height);
	}

	bool GetPlayerInfo(int index, player_info_t* info)
	{
		typedef bool(__thiscall* fn)(void*, int, player_info_t*);
		return VMT::getvfunc<fn>(this, 8)(this, index, info);
	}

	int GetLocalPlayer()
	{
		typedef int(__thiscall* fn)(void*);
		return VMT::getvfunc<fn>(this, 12)(this);
	}

	void ClientCmd(const char* cmd)
	{
		typedef void(__thiscall* fn)(void*, const char*);
		VMT::getvfunc<fn>(this, 108)(this, cmd);
	}

	void SetViewAngles(Vector& angles)
	{
		typedef void(__thiscall* fn)(void*, Vector&);
		VMT::getvfunc<fn>(this, 19)(this, angles);
	}

	void GetViewAngles(Vector& angles)
	{
		typedef void(__thiscall* fn)(void*, Vector&);
		VMT::getvfunc<fn>(this, 18)(this, angles);
	}

	VMatrix WorldToScreenMatrix()
	{
		typedef VMatrix(__thiscall* fn)(void*);
		return VMT::getvfunc<fn>(this, 37)(this);
	}
};
extern CEngine* g_pEngine;