#include "InterfaceManager.hpp"
#include <iostream>
#include <Windows.h>
#include "interface/Engine.h"
#include "interface\EntityList.h"
#include "Address.h"
#include "interface\Globals.h"
#include "interface\Predictionandmovement.h"
#include "interface\ModelInfo.h"
#include "interface\EngineTrace.h"
#include "interface\Panel.h"
#include "interface\Surface.h"

void* g_pClient;
void* g_pClientMode;
void InterfaceManager::GetInterfaces()
{
	g_pClient = GetInterface("client.dll", "VClient");
	g_pEngine = (CEngine*)GetInterface("engine.dll", "VEngineClient");
	g_pClientEntList = (EntityList*)GetInterface("client.dll", "VClientEntityList");
	g_pPrediction = (CPrediction*)GetInterface("client.dll", "VClientPrediction");
	g_pMovement = (CMovement*)GetInterface("client.dll", "GameMovement");
	g_pModelInfo = (SDKModelInfo*)GetInterface("engine.dll", "VModelInfoClient");
	g_pEngineTrace = (SDKTrace*)GetInterface("engine.dll", "EngineTraceClient");
	g_pPanel = (CPanel*)GetInterface("vgui2.dll", "VGUI_Panel");
	g_pSurface = (CSurface*)GetInterface("vguimatsurface.dll", "VGUI_Surface");

	Address CDLLTable = ((Address)g_pClient).To<DWORD*>();

	Address pShutdown = (CDLLTable.As<DWORD*>())[4];
	g_pClientMode = *pShutdown.GetOffset(0xF2).To<DWORD**>();
	printf("Clientmode found 0x%X\n", g_pClientMode);


	Address HudUpdate = (CDLLTable.As<DWORD*>())[11];
	g_pGlobals = HudUpdate.GetOffset(0x10).To<CGlobals*>(); //pGlobals
	printf("Globals found 0x%X\n", g_pGlobals);
}

void* InterfaceManager::GetInterface(char * modulename, char * interfacename)
{
	void*  Interface = nullptr;
	char PossibleInterfaceName[1024];

	CreateInterfaceFn CreateInterface = (CreateInterfaceFn)GetProcAddress(GetModuleHandle(modulename), "CreateInterface");

	for (int i = 1; i < 100; i++)
	{
		sprintf(PossibleInterfaceName, "%s0%d", interfacename, i);
		Interface = (void*)CreateInterface(PossibleInterfaceName, 0);
		if (Interface)
		{
			printf("%s found 0x%X\n", PossibleInterfaceName, Interface);
			break;
		}

		sprintf(PossibleInterfaceName,"%s00%d", interfacename, i);
		Interface = CreateInterface(PossibleInterfaceName, 0);
		if (Interface)
		{
			printf("%s found 0x%X\n", PossibleInterfaceName, Interface);
			break;
		}
	}

	return Interface;
}
