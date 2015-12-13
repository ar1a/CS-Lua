#pragma once
#include "InterfaceBase.h"
class CGlobals : public InterfaceBase
{
public:
	float realtime;
	int framecount;
	float absoluteframetime;
	float curtime;
	float frametime;
	int maxClients;
	int tickcount;
	float interval_per_tick;
	float interpolation_amount;
	int simTicksThisFrame;
	int network_protocol;
};

extern CGlobals *g_pGlobals;