#pragma once
#include "../Vector.h"
#include "../VMT.h"
#include "../math.h"
#include "InterfaceBase.h"

#define DISPSURF_FLAG_SURFACE		(1<<0)
#define DISPSURF_FLAG_WALKABLE		(1<<1)
#define DISPSURF_FLAG_BUILDABLE		(1<<2)
#define DISPSURF_FLAG_SURFPROP1		(1<<3)
#define DISPSURF_FLAG_SURFPROP2		(1<<4)

struct csurface_t
{
	const char		*name;
	short			surfaceProps;
	unsigned short	flags;
};

struct cplane_t
{
	Vector	normal;
	float	dist;
	BYTE	type;
	BYTE	signbits;
	BYTE	pad[2];
};

class CBaseTrace
{
public:
	bool IsDispSurface(void) { return ((dispFlags & DISPSURF_FLAG_SURFACE) != 0); }
	bool IsDispSurfaceWalkable(void) { return ((dispFlags & DISPSURF_FLAG_WALKABLE) != 0); }
	bool IsDispSurfaceBuildable(void) { return ((dispFlags & DISPSURF_FLAG_BUILDABLE) != 0); }
	bool IsDispSurfaceProp1(void) { return ((dispFlags & DISPSURF_FLAG_SURFPROP1) != 0); }
	bool IsDispSurfaceProp2(void) { return ((dispFlags & DISPSURF_FLAG_SURFPROP2) != 0); }

public:
	Vector			startpos;
	Vector			endpos;
	cplane_t		plane;

	float			fraction;

	int				contents;
	unsigned short	dispFlags;

	bool			allsolid;
	bool			startsolid;

	CBaseTrace() {}

private:
	// No copy constructors allowed
	CBaseTrace(const CBaseTrace& vOther);
};
class CGameTrace : public CBaseTrace
{
public:
	bool DidHitWorld() const;

	bool DidHitNonWorldEntity() const;

	int GetEntityIndex() const;

	bool DidHit() const
	{
		return fraction < 1 || allsolid || startsolid;
	}

public:
	float			fractionleftsolid;
	csurface_t		surface;

	int				hitgroup;

	short			physicsbone;
	unsigned short	worldSurfaceIndex;

	void*		m_pEnt;
	int				hitbox;

	CGameTrace()  :
		fractionleftsolid(0),
		hitgroup(0),
		physicsbone(0),
		worldSurfaceIndex(0),
		m_pEnt(0){
		hitbox = 0;
		startpos = Vector(0,0,0);
		endpos = Vector(0, 0, 0);
		fraction = 0;
		contents = 0;
		dispFlags = 0;
		allsolid = 0;
		startsolid = 0;
	}
	CGameTrace(const CGameTrace& vOther)
	{
		fractionleftsolid = vOther.fractionleftsolid;
		surface = vOther.surface;
		hitgroup = vOther.hitgroup;
		physicsbone = vOther.physicsbone;
		worldSurfaceIndex = vOther.worldSurfaceIndex;
		m_pEnt = vOther.m_pEnt;
		hitbox = vOther.hitbox;
		startpos = vOther.startpos;
		endpos = vOther.endpos;
		plane = vOther.plane;
		fraction = vOther.fraction;
		contents = vOther.contents;
		dispFlags = vOther.dispFlags;
		allsolid = vOther.allsolid;
		startsolid = vOther.startsolid;
	}
};
typedef CGameTrace trace_t;
class ITraceFilter
{
public:
	virtual bool ShouldHitEntity(void* pEntity, int contentsMask) = 0;
	virtual int	GetTraceType() const = 0;
};

class CTraceFilter : public ITraceFilter
{
public:
	bool ShouldHitEntity(void* pEntityHandle, int contentsMask)
	{
		return !(pEntityHandle == pSkip);
	}

	virtual int GetTraceType() const
	{
		return 0;
	}

	void* pSkip;
};

struct Ray_t
{
	VectorAligned m_Start;
	VectorAligned m_Delta;
	VectorAligned m_StartOffset;
	VectorAligned m_Extents;

	const matrix3x4_t* m_pWorldAxisTransform;

	bool m_IsRay;
	bool m_IsSwept;

	Ray_t() /*: m_pWorldAxisTransform(NULL)*/ { }

	void Init(Vector& start, Vector& end)
	{
		m_Delta = end - start;

		m_IsSwept = (m_Delta.LengthSqr() != 0);

		m_Extents.x = m_Extents.y = m_Extents.z = 0;
		m_pWorldAxisTransform = NULL;
		m_IsRay = true;

		m_StartOffset.x = m_StartOffset.y = m_StartOffset.z = 0;
		m_Start = start;
	}

};

class SDKTrace : public InterfaceBase
{
public:


	void TraceRay(const Ray_t &ray, unsigned int Make, ITraceFilter* filter, trace_t *trace)
	{
		typedef void(__thiscall* fn)(void*, const Ray_t &, unsigned int, ITraceFilter*, trace_t*);
		VMT::getvfunc<fn>(this, 5)(this, ray, Make, filter, trace);

	}

};


extern SDKTrace* g_pEngineTrace;