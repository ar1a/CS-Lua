#pragma once



typedef void* (*CreateInterfaceFn)(const char *Name, int *ReturnCode);
class InterfaceManager
{
public:
	static void GetInterfaces();

	static void* GetInterface(char* modulename, char* interfacename);
};

extern void* g_pClient;

extern void* g_pClientMode;