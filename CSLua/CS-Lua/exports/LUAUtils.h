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

	Vector WorldToScreen(const Vector& origin) {
		Vector screen(0,0,0);
		//Vector origin = luabridge::LuaRef::fromStack(L, 2).cast<Vector>();  //Vector is correct

		matrix3x4_t w2sMatrix = g_pEngine->WorldToScreenMatrix();
		/*SOMETHING IN HERE CRASHES*/
		//if (!ScreenTransform(origin, screen)) {
			/*int iScreenWidth, iScreenHeight;
			g_pEngine->GetScreenSize(iScreenWidth, iScreenHeight);

			float fScreenX = iScreenWidth / 2.0f;
			float fScreenY = iScreenHeight / 2.0f;

			fScreenX += 0.5f * screen.x * iScreenWidth + 0.5f;
			fScreenY -= 0.5f * screen.y * iScreenHeight + 0.5f;

			screen.x = fScreenX;
			screen.y = fScreenY;*/
		//}
		/*END*/
		return screen;
	}
	bool ScreenTransform(Vector& point, Vector& screen) {
		 matrix3x4_t w2sMatrix = g_pEngine->WorldToScreenMatrix();
		/*screen.x = w2sMatrix.m[0][0] * point.x + w2sMatrix.m[0][1] * point.y + w2sMatrix.m[0][2] * point.z + w2sMatrix.m[0][3];
		screen.y = w2sMatrix.m[1][0] * point.x + w2sMatrix.m[1][1] * point.y + w2sMatrix.m[1][2] * point.z + w2sMatrix.m[1][3];

		float w = w2sMatrix.m[3][0] * point.x + w2sMatrix.m[3][1] * point.y + w2sMatrix.m[3][2] * point.z + w2sMatrix.m[3][3];
		screen.z = 0.0f;

		bool bIsBehind = false;
		/*if (w < 0.001f) {
			bIsBehind = true;
			screen.Init(0, 0, 0);
		}
		else {
			float invw = 1.0f / w;
			screen.x *= invw;
			screen.y *= invw;
		}
		*/
		return true;
	}

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
