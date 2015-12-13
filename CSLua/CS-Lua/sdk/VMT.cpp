#include "VMT.h"

VMT::VMT(void) :
	m_bVftpHooked(false),
	m_bTableHooked(false),
	m_bValidObject(false),
	m_bValidTable(false),
	m_iCount(0),
	m_pbHookedMethods(0),
	m_iHookCount(0),
	m_pCopyTable(0),
	m_pObject(0),
	m_pBackupTable(0)
{

}

VMT::VMT(void* object) :
	m_bVftpHooked(false),
	m_bTableHooked(false),
	m_bValidObject(false),
	m_bValidTable(false),
	m_iCount(0),
	m_pbHookedMethods(0),
	m_iHookCount(0),
	m_pCopyTable(0),
	m_pObject(object),
	m_pBackupTable(0)
{
}

VMT::~VMT(void)
{
	shutdown();
}

bool VMT::init()
{

	if (!countTable())
		return false;

	m_pOriginalTable = *(void**)m_pObject;

	m_pBackupTable = malloc(sizeof(void*) * m_iCount);

	if (!m_pBackupTable)
		return false;

	//Make a backup table purely for backup purposes, not used for hooking at all
	memcpy(m_pBackupTable, m_pOriginalTable, sizeof(void*) * m_iCount);

	m_pbHookedMethods = (bool*)malloc(sizeof(bool) * m_iCount);

	//Make table of bools to designate hook statuses
	memset(m_pbHookedMethods, 0, sizeof(bool) * m_iCount);

	return true;
}

void VMT::shutdown()
{

	if (m_bVftpHooked)
		setTableHook(false);

	if (m_bTableHooked)
		removeHooks();

	if (m_pBackupTable)
		free(m_pBackupTable);

	if (m_pbHookedMethods)
		free(m_pbHookedMethods);

	m_bVftpHooked = false;
	m_bTableHooked = false;
	m_bValidObject = false;
	m_bValidTable = false;
	m_iCount = 0;
	m_pbHookedMethods = 0;
	m_iHookCount = 0;
	m_pCopyTable = 0;
	m_pObject = 0;
	m_pBackupTable = 0;
}

void VMT::removeHooks()
{
	if (!m_bTableHooked || !m_pBackupTable || !isValid())
		return;
	DWORD old;
	VirtualProtect(*(void**)m_pObject, sizeof(void*) * m_iCount, PAGE_EXECUTE_READWRITE, &old);
	memcpy(*(void**)m_pObject, m_pBackupTable, sizeof(void*) * m_iCount);
	VirtualProtect(*(void**)m_pObject, sizeof(void*) * m_iCount, old, &old);
	memset(m_pbHookedMethods, 0, sizeof(bool) * m_iCount);

	m_iHookCount = 0;
	m_bTableHooked = false;
}

void VMT::setTargetObject(void* object)
{
	shutdown();

	m_pObject = object;

	init();
}

void* VMT::getTargetObject()
{
	return m_pObject;
}

bool VMT::setTableHook(bool hooked)
{
	if (!isValid())
		return false;

	//Must remove table hooks before replacing vftp
	if (m_bTableHooked)
		return false;

	if (hooked)
	{
		m_bVftpHooked = true;

		m_pCopyTable = malloc(sizeof(void*) * m_iCount);

		if (!m_pCopyTable)
			return false;

		//Allocate a new table used for hooking
		memcpy(m_pCopyTable, *(void**)m_pObject, sizeof(void*) * m_iCount);

		//replace the _vftp
		*(void**)m_pObject = m_pCopyTable;
	}
	else
	{
		if (!m_bVftpHooked)
			return false;

		m_bVftpHooked = false;

		//free table that was allocated for hooking
		free(m_pCopyTable);

		//replace vftp with old vftp
		*(void**)m_pObject = m_pOriginalTable;
	}
}

void* VMT::hookFunction(int index, void* hookedFunction)
{
	if (!isValid())
		return 0;

	if (index < m_iCount && index >= 0)
	{
		DWORD old = 0;
		void** pTable = (*(void***)m_pObject);
		void* pOrig = pTable[index];

		VirtualProtect(&pTable[index], 4, PAGE_EXECUTE_READWRITE, &old);

		pTable[index] = hookedFunction;

		VirtualProtect(&pTable[index], 4, old, &old);

		//we have installed at least one hook
		m_bTableHooked = true;

		//if a hook wasn't already installed here
		if (!m_pbHookedMethods[index])
		{
			m_pbHookedMethods[index] = true;
			++m_iHookCount;
		}

		return pOrig;
	}

	return 0;
}


bool VMT::unhookFunction(int index)
{
	if (!isValid())
		return false;

	if (index < m_iCount && index >= 0)
	{
		void* OrigFunc = ((void**)m_pOriginalTable)[index];
		if (hookFunction(index, OrigFunc))
		{
			m_pbHookedMethods[index] = false;
			--m_iHookCount;

			if (m_iHookCount == 0)
				m_bTableHooked = false;

			return true;
		}
	}

	return false;
}

unsigned int VMT::countTable()
{
	if (!m_pObject)
		return 0;
	//guess It was incorrect casting to BaseInterface** But 
	for (m_iCount = 0; !IsBadReadPtr((*(void***)m_pObject)[m_iCount], 4); m_iCount++);

	return m_iCount;//its right here. m_pObject is access violation. i think its out of range or something. sec bp+ run
}

bool VMT::isValid()
{
	if (IsBadReadPtr(m_pObject, 4))
		return false;

	if (IsBadReadPtr(*(void**)m_pObject, 4))
		return false;

	if (m_iCount == 0)
		return false;

	return true;
}

void* VMT::getOriginalFunction(int index)
{
	if (index < m_iCount && index >= 0) {
		return ((DWORD**)m_pBackupTable)[index];
	}

	return 0;
}