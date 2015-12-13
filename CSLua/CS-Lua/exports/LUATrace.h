#pragma once
#include "BaseLuaExport.h"
#include "../sdk/interface/EngineTrace.h"
#include "LUAEntity.h"

class LUAtrace_t : public BaseLuaExport
{
public:
	LUAtrace_t(trace_t trace)
	{
		_trace = trace;
	}

	bool DidHit()
	{
		return _trace.DidHit();
	}

	bool IsVisible()
	{
		return _trace.fraction > .97f;
	}

	bool DidHitEntity(LUAEntity ent)
	{
		if (!(CBaseEntity*)ent)
			return false;
		return _trace.m_pEnt == ((CBaseEntity*)ent);
	}

	int GetHitbox()
	{
		return _trace.hitgroup;
	}

	Vector GetEndPos()
	{
		return _trace.endpos;
	}
	LUAEntity GetEntity()
	{
		return (CBaseEntity*)_trace.m_pEnt;
	}

private:
	trace_t _trace;
};

class LUATrace : public BaseLuaExport
{
public:
	LUATrace(SDKTrace* trace)
	{
		_trace = trace;
	}

	LUAtrace_t TraceRay(Vector start, Vector end, LUAEntity skipent)
	{
		if (!start.IsValid() || !end.IsValid() || !(CBaseEntity*)skipent)
			return LUAtrace_t(trace_t());
		trace_t tr;
		Ray_t ray;
		CTraceFilter filter;
		filter.pSkip = (CBaseEntity*)skipent;

		ray.Init(start, end);
		_trace->TraceRay(ray, 0x4600400B, &filter, &tr);
		return LUAtrace_t(tr);
	}



private:
	SDKTrace* _trace;
};