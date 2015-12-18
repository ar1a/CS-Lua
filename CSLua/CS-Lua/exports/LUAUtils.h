#pragma once
#include "BaseLuaExport.h"
#include "../sdk/interface/Engine.h"
#include "../sdk/Vector.h"
#include "../sdk/CBaseEntity.h"
#include "../sdk/interface/ModelInfo.h"
#include "Exports.h"

class LUAUtils : public BaseLuaExport
{

public:
	bool IsPlayer(LUAEntity ent)
	{
		player_info_t info;
		return (g_pEngine->GetPlayerInfo(((CBaseEntity*)ent)->GetIndex(), &info));
	}
	Vector GetHitboxPosition(int Hitbox, LUAEntity ent)
	{
		CBaseEntity* target = (CBaseEntity*)ent;
		void* model = target->GetModel();
		if (!model)
			return Vector(0, 0, 0);

		studiohdr_t* hdr = g_pModelInfo->GetStudioModel(model);
		if (!hdr)
			return Vector(0, 0, 0);

		matrix3x4_t matrix[128];
		if (!target->SetupBones(matrix, 128, 0x100, 0))
			return Vector(0, 0, 0);

		mstudiohitboxset_t *set = hdr->pHitboxSet(target->GetHitboxSet());
		if (!set)
			return Vector(0, 0, 0);

		mstudiobbox_t* box = set->pHitbox(Hitbox);

		if (!box)
			return Vector(0, 0, 0);

		Vector center = ((box->bbmin + box->bbmax) * .5f);
		Vector hitboxpos;
		VectorTransform(center, matrix[box->bone], hitboxpos);

		return hitboxpos;
	}

	LUAEntity LocalPlayer()
	{
		return LUAEntity(g_pClientEntList->GetClientEntity(g_pEngine->GetLocalPlayer()));
	}

private:
	__forceinline float DotProduct(const Vector v1, const float* v2)
	{
		return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
	}

	void VectorTransform(const Vector in1, const matrix3x4_t& in2, Vector& out)
	{
		out[0] = DotProduct(in1, in2[0]) + in2[0][3];
		out[1] = DotProduct(in1, in2[1]) + in2[1][3];
		out[2] = DotProduct(in1, in2[2]) + in2[2][3];
	}

};
extern LUAUtils g_Utils;
