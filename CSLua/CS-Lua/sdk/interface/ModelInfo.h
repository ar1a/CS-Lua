#pragma once
#include "../VMT.h"
#include "../Vector.h"

#include "InterfaceBase.h"
struct mstudiobbox_t
{
	int					bone;
	int					group;
	Vector				bbmin;
	Vector				bbmax;
	int					szhitboxnameindex;
	int					m_iPad01[3];
	float				m_flRadius;
	int					m_iPad02[4];
};

struct mstudiohitboxset_t
{
	int					sznameindex;
	inline char * const	pszName(void) const { return ((char *)this) + sznameindex; }
	int					numhitboxes;
	int					hitboxindex;
	inline mstudiobbox_t *pHitbox(int i) const { return (mstudiobbox_t*)(((unsigned char*)this) + hitboxindex) + i; };
};

struct studiohdr_t
{
	unsigned char				pad00[12];
	char				name[64];
	unsigned char				pad01[80];
	int					numbones;
	int					boneindex;
	unsigned char				pad02[12];
	int					hitboxsetindex;
	unsigned char				pad03[228];

	// Look up hitbox set by index
	inline mstudiohitboxset_t	*pHitboxSet(int i) const
	{
		return (mstudiohitboxset_t *)(((unsigned char *)this) + hitboxsetindex) + i;
	};

	// Calls through to hitbox to determine size of specified set
	inline mstudiobbox_t *pHitbox(int i, int set) const
	{
		const mstudiohitboxset_t *s = pHitboxSet(set);
		if (!s)
			return NULL;

		return s->pHitbox(i);
	};

	// Calls through to set to get hitbox count for set
	inline int			iHitboxCount(int set) const
	{
		const mstudiohitboxset_t *s = pHitboxSet(set);
		if (!s)
			return 0;

		return s->numhitboxes;
	};
};

class SDKModelInfo : public InterfaceBase
{
public:
	studiohdr_t* GetStudioModel(void* model)
	{
		typedef studiohdr_t*(__thiscall* fn)(void*, void*);
		return VMT::getvfunc<fn>(this, 30)(this, model);
	}
};

extern SDKModelInfo* g_pModelInfo;
