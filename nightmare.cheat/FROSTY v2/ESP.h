#pragma once
#include "SDK.h"
#include "singleton.hpp"
#include "Color.h"
#include "MathFunctions.h"
#include "bulletshow.h"



class visuals
	:public singleton<visuals>
{
public:
	visuals();
	void OnPaintTraverse(C_BaseEntity* local);
	void FlashTimer(C_BaseEntity * local);
	void AmbientLight();
	static void Hitbox(int index);

	struct ESPBox
	{
		int x, y, w, h;
	};
	std::vector<cbullet_tracer_info>logs;
	void renderBeams();
	void DrawLogs();
	struct loginfo {
		loginfo(std::string text,
			Color color,
			float time) {
			this->text = text;
			this->color = color;
			this->time = time;
		}

		std::string text;
		Color color;
		float time;
	};
	std::deque<loginfo>events;

	struct Footstep {
		long expiration;
		int entityId;
		Vector position;
	};

	std::vector<Footstep> footsteps;
	static bool IsVisibleScan(C_BaseEntity *player)
	{
		C_BaseEntity* g_LocalPlayer = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
		matrix3x4 boneMatrix[MAXSTUDIOBONES];
		Vector eyePos = g_LocalPlayer->GetEyePosition();

		CGameTrace tr;
		Ray_t ray;
		CTraceFilter filter;
		filter.pSkip = g_LocalPlayer;

		if (!player->SetupBones(boneMatrix, MAXSTUDIOBONES, BONE_USED_BY_Hitbox, 0.0f))
		{
			return false;
		}

		auto studio_model = g_ModelInfo->GetStudiomodel(player->GetModel());
		if (!studio_model)
		{
			return false;
		}

		int scan_Hitboxes[] = {
			Head,
			Stomach,
			Pelvis,
			RightFoot,
			LeftFoot,
			RightHand,
			LeftHand
		};

		for (int i = 0; i < ARRAYSIZE(scan_Hitboxes); i++)
		{
			auto Hitbox = studio_model->GetHitboxSet(player->GetHitboxSet())->pHitbox(scan_Hitboxes[i]);
			if (Hitbox)
			{
				auto
					min = Vector{},
					max = Vector{};

				VectorTransform(Hitbox->bbmin, boneMatrix[Hitbox->bone], min);
				VectorTransform(Hitbox->bbmax, boneMatrix[Hitbox->bone], max);

				ray.Init(eyePos, (min + max) * 0.5);
				g_EngineTrace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr);

				if (tr.hit_entity == player || tr.fraction > 0.97f)
					return true;
			}
		}
		return false;
	}

private:
	C_BaseEntity * BombCarrier;
	void DrawBombPlanted(C_BaseEntity * entity, C_BaseEntity * local);
	void DrawBomb(C_BaseEntity * entity, ClientClass * cClass);
	void armorbar(RECT rect, C_BaseEntity * pEntity);
	void DrawBox(RECT rect, Color Col, C_BaseEntity * entity);
	void RenderFill(RECT rect);
	void DrawCorners(RECT rect, Color Col, C_BaseEntity * entity);
	void ThreeDBox(Vector minin, Vector maxin, Vector pos, Color Col);
	void OffscreenArrow(C_BaseEntity * entity);
	void AmmoBox(RECT rect, C_BaseEntity * pPlayer, Color color);
	void DrawInfo(RECT rect, C_BaseEntity * entity, C_BaseEntity * local);
	void DrawArrows(C_BaseEntity * local);
	void DrawAngles();
	bool GetBBox(C_BaseEntity * entity, visuals::ESPBox & result);
	void RenderWeapon(C_BaseEntity * entity, Color col);
	void DrawTaserRange();
	void BulletTrace(C_BaseEntity* pEntity, Color color);
	void Skeleton(C_BaseEntity* pEntity, Color Color);
	void HealthSkeleton(C_BaseEntity * pEntity);
	void DrawSnapLine(RECT rect, C_BaseEntity * entity);
	void DrawHealth(RECT rect, C_BaseEntity* entity);
	RECT DynamicBox(C_BaseEntity* pPlayer, bool& PVS, C_BaseEntity* local);
	void SkyChanger();
	void Hitmarker();
	void NightMode();
	void DoAsusWalls();


};

inline float CSGO_Armor(float flDamage, int ArmorValue)
{
	float flArmorRatio = 0.5f;
	float flArmorBonus = 0.5f;
	if (ArmorValue > 0) {
		float flNew = flDamage * flArmorRatio;
		float flArmor = (flDamage - flNew) * flArmorBonus;

		if (flArmor > static_cast<float>(ArmorValue)) {
			flArmor = static_cast<float>(ArmorValue) * (1.f / flArmorBonus);
			flNew = flDamage - flArmor;
		}

		flDamage = flNew;
	}
	return flDamage;
}


namespace ColorAlpha
{
	struct VisualsStruct
	{
		Color2 clr_fill;
		C_BaseEntity* player;
	};

	extern VisualsStruct ESP_ctx;
	extern Color2 clr_fill;
}




char* const itemIcons[] =
{
	"3", //0 - default
	"A", //1
	"B", //2
	"C", //3
	"D", //4
	"none", //5
	"none", //6
	"W", //7
	"U", //8
	"Z", //9

	"R", //10
	"X", //11
	"none", //12
	"Q", //13
	"g", //14
	"none", //15
	"S", //16
	"K", //17
	"none", //18
	"P", //19

	"none", //20
	"none",
	"none",
	"none",
	"L",
	"b",
	"M",
	"d",
	"f",
	"c",

	"H", //30
	"h",
	"E",
	"N",
	"O",
	"e",
	"F",
	"none",
	"Y",
	"V",

	"a", //40
	"1",
	"1",
	"i",
	"j",
	"k",
	"l",
	"m",
	"n",
	"o",

	"none", //50
	"none",
	"none",
	"none",
	"none",
	"none",
	"none",
	"none",
	"none",
	"knife",

	"T", //60
	"G",
	"none",
	"I",
	"J"
};

inline const char* ItemDefinitionIndexxToString2(int index)
{
	if (index < 0 || index > 64)
		index = 0;

	return itemIcons[index];
}


char* const itemNames[] =
{

	"KNIFE", //0 - default
	"DEAGLE",
	"ELITE",
	"FIVESEVEN",
	"GLOCK",
	"none",
	"none",
	"AK-47",
	"AUG",
	"AWP",

	"FAMAS", //10
	"G3SG1",
	"none",
	"GALIL",
	"M249",
	"none",
	"M4A4",
	"MAC-10",
	"none",
	"P90",

	"none", //20
	"none",
	"none",
	"none",
	"UMP-45",
	"XM1014",
	"BIZON",
	"MAG-7",
	"NEGEV",
	"SAWED-OFF",

	"TEC-9", //30
	"Zeus-x27",
	"P2000",
	"MP-7",
	"MP-9",
	"NOVA",
	"P250",
	"none",
	"SCAR-20",
	"SG-556",

	"SSG-08", //40
	"KNIFE",
	"KNIFE",
	"FLASH",
	"NADE",
	"SMOKE",
	"MOLOTOV",
	"DECOY",
	"INCENDIARY",
	"C4",

	"none", //50
	"none",
	"none",
	"none",
	"none",
	"none",
	"none",
	"none",
	"none",
	"KNIFE",

	"M4A1", //60
	"USP",
	"none",
	"CZ-75",
	"REVOLVER"
};




inline const char* ItemDefinitionIndexxToString(int index)
{
	if (index < 0 || index > 64)
		index = 0;

	return itemNames[index];
}
