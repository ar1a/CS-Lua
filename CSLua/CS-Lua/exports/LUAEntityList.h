#pragma once
#include "BaseLuaExport.h"
#include "LUAEntity.h"
#include "../sdk/interface/EntityList.h"

class LUAEntityList : public BaseLuaExport
{
public:
	LUAEntityList(EntityList* list) : m_pEntityList(list) {}
	LUAEntity GetEntity(int index)
	{
		return LUAEntity(m_pEntityList->GetClientEntity(index));
	}

	int GetHighestEntityIndex()
	{
		return m_pEntityList->GetHighestEntityIndex();
	}

private:
	EntityList* m_pEntityList;
};
