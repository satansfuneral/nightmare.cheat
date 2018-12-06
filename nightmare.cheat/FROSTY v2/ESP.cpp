#include "ESP.h"
#include "Interfaces.h"
#include "Render.h"
#include <ctime>
#include <iostream>
#include <algorithm>
#include "GrenadePrediction.h"
#include "memoryfonts.h"
#include "LagComp.h"
#include "Autowall.h"
#include "Hooks.h"

namespace ColorAlpha
{
	VisualsStruct ESP_ctx;
	RECT bbox;
}

//int alpha[64];

visuals::visuals()
{
	BombCarrier = nullptr;
}
Color box(114, 114, 114, 114);
Color fill(0, 0, 0);
Color snapline(114, 114, 114, 114);
Color bullettrace (114, 114, 114, 114);
Color headpos(114, 114, 114, 114);
Color skele(114, 114, 114, 114);
Color ammobox(114, 114, 114, 114);
void VectorTrans(const float *in1, const matrix3x4_t& in2, float *out)
{
	out[0] = DotProduct(in1, in2[0]) + in2[0][3];
	out[1] = DotProduct(in1, in2[1]) + in2[1][3];
	out[2] = DotProduct(in1, in2[2]) + in2[2][3];
}
void VectorTrans(const Vector& in1, const matrix3x4_t& in2, Vector& out)
{
	VectorTrans(&in1.x, in2, &out.x);
}

int width = 0;
int height = 0;
bool done = false;
float damage;
char bombdamagestringdead[24];
char bombdamagestringalive[24];
RECT GetBox(C_BaseEntity* ent, Vector pointstransf[])
{
	RECT rect{};
	auto collideable = ent->GetCollideable();

	if (!collideable)
		return rect;

	auto min = collideable->OBBMins();
	auto max = collideable->OBBMaxs();

	const matrix3x4_t &trans = ent->m_rgflCoordinateFrame();

	Vector points[] =
	{
		Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z),
		Vector(max.x, min.y, max.z)
	};

	Vector pointsTransformed[8];
	for (int i = 0; i < 8; i++) {
		VectorTrans(points[i], trans, pointsTransformed[i]);
	}

	Vector pos = ent->GetVecOrigin();
	Vector screen_points[8] = {};

	for (int i = 0; i < 8; i++)
		if (!g_Render->WorldToScreen(pointsTransformed[i], screen_points[i]))
			return rect;
		else
			pointstransf[i] = screen_points[i];

	auto left = screen_points[0].x;
	auto top = screen_points[0].y;
	auto right = screen_points[0].x;
	auto bottom = screen_points[0].y;

	for (int i = 1; i < 8; i++)
	{
		if (left > screen_points[i].x)
			left = screen_points[i].x;
		if (top < screen_points[i].y)
			top = screen_points[i].y;
		if (right < screen_points[i].x)
			right = screen_points[i].x;
		if (bottom > screen_points[i].y)
			bottom = screen_points[i].y;
	}
	return RECT{ (long)left, (long)top, (long)right, (long)bottom };
}

void visuals::OnPaintTraverse(C_BaseEntity* local)
{
	if (!g_Options.Visuals.Enabled)
		return;

	CInput::CUserCmd* pCmd;
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)g_EntityList->GetClientEntityFromHandle(local->GetActiveWeaponHandle());
	C_BaseEntity* local_observed = (C_BaseEntity*)g_EntityList->GetClientEntityFromHandle(local->GetObserverTargetHandle());

	AmbientLight();
	NightMode();
	DoAsusWalls();
	renderBeams();
	SkyChanger();
	DrawLogs();

	CCSGrenadeHint::instance().Paint();

	if (local->IsAlive() && g_Options.Visuals.angleLines && g_Input->m_fCameraInThirdPerson)
		DrawAngles();

	DrawArrows(local);

	if (g_Options.Misc.Hitmarker == 1 && g_Engine->IsConnected() && g_Engine->IsInGame() && local->IsAlive())
		Hitmarker();

	for (int i = 0; i < g_EntityList->GetHighestEntityIndex(); i++)
	{
		C_BaseEntity *entity = g_EntityList->GetClientEntity(i);

		if (!entity)
			continue;

		if (entity->IsDormant())
		{
			Color(g_Options.Colors.TeamESP) = box;
			Color(g_Options.Colors.EnemyESP) = box;
			Color(g_Options.Colors.BoxFillEnemy) = fill;
			Color(g_Options.Colors.BoxFillTeam) = fill;
			Color(g_Options.Colors.AimLineColor) = snapline;
			Color(g_Options.Colors.HeadLine) = bullettrace;
			Color(g_Options.Colors.HeadPosition) = headpos;
			Color(g_Options.Colors.Skeleton) = skele;
			Color(g_Options.Colors.AmmoBox) = ammobox;
		}
		else
		{
			Color(g_Options.Colors.TeamESP) = Color(g_Options.Colors.TeamESP);
			Color(g_Options.Colors.EnemyESP) = Color(g_Options.Colors.EnemyESP);
			Color(g_Options.Colors.BoxFillEnemy) = Color(g_Options.Colors.BoxFillEnemy);
			Color(g_Options.Colors.BoxFillTeam) = Color(g_Options.Colors.BoxFillTeam);
			Color(g_Options.Colors.AimLineColor) = Color(g_Options.Colors.AimLineColor);
			Color(g_Options.Colors.HeadLine) = Color(g_Options.Colors.HeadLine);
			Color(g_Options.Colors.HeadPosition) = Color(g_Options.Colors.HeadPosition);
			Color(g_Options.Colors.Skeleton) = Color(g_Options.Colors.Skeleton);
			Color(g_Options.Colors.AmmoBox) = Color(g_Options.Colors.AmmoBox);
		}

		if (entity == local_observed)
		{
			if (local->GetObserverMode() == (ObserverMode_t*)((int)ObserverMode_t::OBS_MODE_DEATHCAM || (int)ObserverMode_t::OBS_MODE_IN_EYE || (int)ObserverMode_t::OBS_MODE_CHASE || (int)ObserverMode_t::OBS_MODE_FIXED))
				continue;
		}

		if (entity == local)
			continue;

		if (g_Options.Misc.radaringame)
			*(char*)((DWORD)(entity)+offsetz.DT_BaseEntity.m_bSpotted) = 1;

		if (g_Options.Visuals.OffscreenIndicator)
			OffscreenArrow(entity);

		if (g_Options.Visuals.TaserRange)
			DrawTaserRange();


		FlashTimer(local);

		ClientClass* cClass = (ClientClass*)entity->GetClientClass();
		player_info_t pinfo;
		Vector max = entity->GetCollideable()->OBBMaxs();
		Vector pos, pos3D;
		pos3D = entity->GetVecOrigin();
		bool PVS = false;
		RECT rect = DynamicBox(entity, PVS, local);

		if (g_Options.Visuals.DroppedGuns &&  cClass->m_ClassID != (int)ClassID::CBaseWeaponWorldModel && ((strstr(cClass->m_pNetworkName, "Weapon") || cClass->m_ClassID == (int)ClassID::CDEagle || cClass->m_ClassID == (int)ClassID::CAK47)))
		{
			Color clr = Color(g_Options.Colors.DroppedWeapons);

			RenderWeapon(entity, clr);
		}
		if (g_Options.Visuals.C4World)
		{
			if (cClass->m_ClassID == (int)ClassID::CPlantedC4)
				DrawBombPlanted(entity, local);
		}
		if (cClass->m_ClassID == (int)ClassID::CC4)
		{
			DrawBomb(entity, cClass);
		}
		if (g_Options.Visuals.Hostage)
		{
			Color clr = Color(255, 255, 255);
			if (cClass->m_ClassID == (int)ClassID::CHostage)
			{
				g_Render->DrawString2(g_Render->font.ESP, (int)pos.x, (int)pos.y, Color(255, 255, 255, 255), FONT_CENTER, "Hostage");

				Vector pos3D = entity->GetAbsOrigin2();

				if (pos3D.x == 0.0f && pos3D.y == 0.0f && pos3D.z == 0.0f)
					return;

				if (g_Options.Visuals.HostageBox)
					DrawBox(rect, clr, entity);
			}
		}
		if (g_Options.Visuals.Chicken)
		{
			Color clr = Color(255, 255, 255);
			if (cClass->m_ClassID == (int)ClassID::CChicken)
			{
				g_Render->DrawString2(g_Render->font.ESP, (int)pos.x, (int)pos.y, Color(255, 255, 255, 255), FONT_CENTER, "Chicken");

				Vector pos3D = entity->GetAbsOrigin2();

				if (pos3D.x == 0.0f && pos3D.y == 0.0f && pos3D.z == 0.0f)
					return;

				if (g_Options.Visuals.ChickenBox)			
					ThreeDBox(entity->GetCollideable()->OBBMins(), entity->GetCollideable()->OBBMaxs(), entity->GetVecOrigin(), clr);
				
			}
		}
		if (g_Options.Visuals.Grenades)
		{
			if (!g_Render->WorldToScreen(pos3D, pos))
				continue;

			Color GrenadeColor = Color(0, 0, 0, 0);
			char* szModelName = "Unknown Projectile";
			char* szModelIcon = "";
			if (strstr(cClass->m_pNetworkName, XorStr("Projectile")))
			{
				const model_t* pModel = entity->GetModel();
				if (!pModel)
					return;


				const studiohdr_t* pHdr = g_ModelInfo->GetStudiomodel(pModel);

				if (!pHdr)
					return;

				if (!strstr(pHdr->name, XorStr("thrown")) && !strstr(pHdr->name, XorStr("dropped")))
					return;


				IMaterial* mats[32];
				g_ModelInfo->GetModelMaterials(pModel, pHdr->numtextures, mats);

				for (int i = 0; i < pHdr->numtextures; i++)
				{
					IMaterial* mat = mats[i];
					if (!mat)
						continue;

					if (strstr(mat->GetName(), "flashbang"))
					{
						GrenadeColor = Color(255, 255, 0, 255);
						szModelName = "Flashbang";
						szModelIcon = "i";
					}
					else if (strstr(mat->GetName(), "m67_grenade") || strstr(mat->GetName(), "hegrenade"))
					{
						szModelName = "Grenade";
						szModelIcon = "j";
						GrenadeColor = Color(255, 0, 0, 255);
						break;
					}
					else if (strstr(mat->GetName(), "smoke"))
					{
						szModelName = "Smoke";
						szModelIcon = "k";
						GrenadeColor = Color(0, 255, 0, 255);
						break;
					}
					else if (strstr(mat->GetName(), "decoy"))
					{
						szModelName = "Decoy";
						szModelIcon = "m";
						GrenadeColor = Color(0, 255, 0, 255);
						break;
					}
					else if (strstr(mat->GetName(), "incendiary"))
					{
						szModelName = "Incendiary";
						szModelIcon = "n";
						GrenadeColor = Color(255, 0, 0, 255);
						break;
					}
					else if (strstr(mat->GetName(), "molotov"))
					{
						szModelName = "Molotov";
						szModelIcon = "l";
						GrenadeColor = Color(255, 0, 0, 255);
						break;
					}
				}
				switch (g_Options.Visuals.Grenades)
				{
				case 0:
					if (g_Options.Visuals.GrenadeBox)
					{
						ThreeDBox(entity->GetCollideable()->OBBMins(), entity->GetCollideable()->OBBMaxs(), entity->GetVecOrigin(), GrenadeColor);
					}
					break;
				case 1:
					if (!g_Options.Visuals.GrenadeBox)
					{
						g_Render->DrawString2(g_Render->font.ESP, (int)pos.x, (int)pos.y, GrenadeColor, FONT_CENTER, "%s", szModelName);
					}
					else if (g_Options.Visuals.GrenadeBox)
					{
						//						g_Render->DrawOutlinedRect((int)pos.x - 10, (int)pos.y - 10, 20, 20, GrenadeColor);
						g_Render->DrawString2(g_Render->font.ESP, (int)pos.x, (int)pos.y + 15, GrenadeColor, FONT_CENTER, "%s", szModelName);
						ThreeDBox(entity->GetCollideable()->OBBMins(), entity->GetCollideable()->OBBMaxs(), entity->GetVecOrigin(), GrenadeColor);
					}
					break;
				case 2:
					if (!g_Options.Visuals.GrenadeBox)
					{
						g_Render->DrawString2(g_Render->font.Icon, (int)pos.x, (int)pos.y, GrenadeColor, FONT_CENTER, "%s", szModelIcon);
					}

					else if (g_Options.Visuals.GrenadeBox)
					{
						//						g_Render->DrawOutlinedRect((int)pos.x - 10, (int)pos.y - 10, 20, 20, GrenadeColor);
						g_Render->DrawString2(g_Render->font.Icon, (int)pos.x, (int)pos.y + 15, GrenadeColor, FONT_CENTER, "%s", szModelIcon);
						ThreeDBox(entity->GetCollideable()->OBBMins(), entity->GetCollideable()->OBBMaxs(), entity->GetVecOrigin(), GrenadeColor);
					}
					break;
				}
			}
		}

		int owner = 0;

		if (!g_Render->WorldToScreen(pos3D, pos))
			continue;

		if (g_Engine->GetPlayerInfo(i, &pinfo) && entity->IsAlive())
		{
			if (g_Options.Visuals.VisibleOnly && IsVisibleScan(entity) == false)
				continue;

			Color backtrackcolor(g_Options.Colors.Backtrack);
			if (g_Options.Backtrack.backtrackenable)
			{
				if (local->IsAlive())
				{
					for (int t = 0; t < g_Options.Backtrack.backtrackticks; ++t)
					{
						studiohdr_t* studio_hdr = g_ModelInfo->GetStudiomodel(entity->GetModel());

						Vector screenbacktrack[64][12];

						if (headPositions[i][t].simtime && headPositions[i][t].simtime + 1 > local->GetSimulationTime())
						{

							if (g_Render->WorldToScreen(headPositions[i][t].HitboxPos, screenbacktrack[i][t]))
							{
								switch (g_Options.Backtrack.BacktrackType)
								{
								case 1:
									g_Surface->DrawSetColor(backtrackcolor);
									g_Surface->DrawOutlinedRect(screenbacktrack[i][t].x, screenbacktrack[i][t].y, screenbacktrack[i][t].x + 2, screenbacktrack[i][t].y + 2);
									break;
								case 2:

									break;
								}
							}
						}

					}
				}
				else
				{
					memset(&headPositions[0][0], 0, sizeof(headPositions));
				}
			}
			if (g_Options.Ragebot.FakeLagFix)
			{
				if (local->IsAlive())
				{
					Vector screenbacktrack[64];

					if (backtracking->records[i].tick_count + 12 > g_Globals->tickcount)
					{
						if (g_Render->WorldToScreen(backtracking->records[i].HeadPosition, screenbacktrack[i]))
						{

							g_Surface->DrawSetColor(g_Options.Colors.Backtrack);
							g_Surface->DrawLine(screenbacktrack[i].x, screenbacktrack[i].y, screenbacktrack[i].x + 2, screenbacktrack[i].y + 2);

						}
					}
				}
				else
				{
					memset(&backtracking->records[0], 0, sizeof(backtracking->records));
				}
			}

			if (!g_Render->WorldToScreen(pos3D, pos))
				continue;

			Color clr;

			if (!entity->IsDormant())
			{
				clr = entity->GetTeamNum() == local->GetTeamNum() ?
					Color(g_Options.Colors.TeamESP[0] * 255, g_Options.Colors.TeamESP[1] * 255, g_Options.Colors.TeamESP[2] * 255, 255) :
					Color(g_Options.Colors.EnemyESP[0] * 255, g_Options.Colors.EnemyESP[1] * 255, g_Options.Colors.EnemyESP[2] * 255, 255);
			}
			else
			{
				clr = box;
			}

			if (entity->GetTeamNum() == local->GetTeamNum() && !g_Options.Visuals.TeamESP)
				continue;

			if (!entity->IsAlive())
				continue;

			if (g_Options.Visuals.AimLine)
				DrawSnapLine(rect, entity);

			DrawInfo(rect, entity, local);

			if (g_Options.Visuals.Box)
			{
				switch (g_Options.Visuals.BoxType)
				{
				case 0:
					DrawBox(rect, clr, entity);
					break;
				case 1:
					DrawCorners(rect, clr, entity);
					break;
				case 2:
					ThreeDBox(entity->GetCollideable()->OBBMins(), entity->GetCollideable()->OBBMaxs(), entity->GetVecOrigin(), clr);
					break;
				}
			}
			if (g_Options.Visuals.fill)
			{
				if (!entity->IsDormant())
				{

					if (entity->GetTeamNum() == local->GetTeamNum())
					{
						g_Surface->DrawSetColor(Color(g_Options.Colors.BoxFillEnemy[0], g_Options.Colors.BoxFillEnemy[1], g_Options.Colors.BoxFillEnemy[2], g_Options.Visuals.esp_fill_amount));
						ColorAlpha::ESP_ctx.clr_fill.SetAlpha(g_Options.Visuals.esp_fill_amount * 255);
						RenderFill(rect);
					}
					else if (entity->GetTeamNum() != local->GetTeamNum())
					{
						g_Surface->DrawSetColor(Color(g_Options.Colors.BoxFillEnemy[0], g_Options.Colors.BoxFillEnemy[1], g_Options.Colors.BoxFillEnemy[2], g_Options.Visuals.esp_fill_amount));
						ColorAlpha::ESP_ctx.clr_fill.SetAlpha(g_Options.Visuals.esp_fill_amount * 255);
						RenderFill(rect);
					}
				}
			}

			if (g_Options.Visuals.skeletonenbl)
			{
				switch (g_Options.Visuals.skeletonopts)
				{
				case 0:
					Skeleton(entity, skele);
					break;
				case 1:
					HealthSkeleton(entity);
					break;
				}
			}

			if (g_Options.Visuals.health)
				DrawHealth(rect, entity);

			if (g_Options.Visuals.armor)
				armorbar(rect, entity);

			if (g_Options.Visuals.AmmoBox)
				AmmoBox(rect, entity, Color(g_Options.Colors.AmmoBox));

			if (g_Options.Visuals.barrel)
				BulletTrace(entity, bullettrace);

			if (g_Options.Visuals.HeadPosition)
			{
				Color headcolor;

				if (entity->IsDormant())
					headcolor = headpos;
				else
					headcolor = Color(g_Options.Colors.HeadPosition);

				Vector headpos = entity->GetBonePos(8);
				switch (g_Options.Visuals.HeadPosition)
				{
				case 1:
					g_Render->DrawOutlinedRect(headpos.x + g_Options.Visuals.HeadPositionSize, headpos.y + g_Options.Visuals.HeadPositionSize, headpos.x - g_Options.Visuals.HeadPositionSize, headpos.y - g_Options.Visuals.HeadPositionSize, headcolor);
					break;
				case 2:
					g_Surface->DrawSetColor(headcolor);
					g_Surface->DrawFilledCircle(headpos.x, headpos.y, g_Options.Visuals.HeadPositionSize, 100);
					break;
				case 3:
					g_Render->Draw3DCube(g_Options.Visuals.HeadPositionSize, *entity->GetEyeAngles(), headpos, headcolor);
					break;
				}
			}
			
		}

	}
}



void visuals::FlashTimer(C_BaseEntity* local)
{
	if (!g_Options.Visuals.FlashTimer)
		return;

	if (!local)
		return;

	int width, height;
	g_Engine->GetScreenSize(width, height);
	int center_x = width / 2;
	int center_y = height / 2;

	auto flashtime = local->GetFlashDuration();

	if (local->IsFlashed())
	{
		g_Render->DrawPercentCircle(center_x, center_y / 2, 12, flashtime, Color(209, 209, 209));
	}
}



void visuals::AmbientLight()
{
	ConVar* AmbientRedCvar = g_CVar->FindVar("mat_ambient_light_r");
	*(float*)((DWORD)&AmbientRedCvar->fnChangeCallback + 0xC) = NULL;

	ConVar* AmbientGreenCvar = g_CVar->FindVar("mat_ambient_light_g");
	*(float*)((DWORD)&AmbientGreenCvar->fnChangeCallback + 0xC) = NULL;

	ConVar* AmbientBlueCvar = g_CVar->FindVar("mat_ambient_light_b");
	*(float*)((DWORD)&AmbientBlueCvar->fnChangeCallback + 0xC) = NULL;

	if (g_Options.Visuals.ambientlight)
	{

		float AmbientRedAmount = g_Options.Colors.AmbientLight[0];
		float AmbientGreenAmount = g_Options.Colors.AmbientLight[1];
		float AmbientBlueAmount = g_Options.Colors.AmbientLight[2];
		
		AmbientRedCvar->SetValue(AmbientRedAmount);

		AmbientGreenCvar->SetValue(AmbientGreenAmount);

		AmbientBlueCvar->SetValue(AmbientBlueAmount);
	}

	else if (!g_Options.Visuals.ambientlight || !g_Options.Visuals.Enabled)
	{
		AmbientRedCvar->SetValue(0.f);


		AmbientGreenCvar->SetValue(0.f);

		AmbientBlueCvar->SetValue(0.f);
	}

}

void visuals::DrawBombPlanted(C_BaseEntity* entity, C_BaseEntity* local)
{
	BombCarrier = nullptr;

	float damage;
	char bombdamagestringdead[24];
	char bombdamagestringalive[24];

	Vector vOrig; Vector vScreen;
	vOrig = entity->GetVecOrigin();
	CCSBomb* Bomb = (CCSBomb*)entity;
	float flBlow = Bomb->GetC4BlowTime();
	float lifetime = flBlow - (g_Globals->interval_per_tick * local->GetTickBase());
	if (g_Render->WorldToScreen(vOrig, vScreen))
	{
		if (local->IsAlive())
		{
			float flDistance = local->GetEyePosition().DistTo(entity->GetEyePosition());
			float a = 450.7f;
			float b = 75.68f;
			float c = 789.2f;
			float d = ((flDistance - b) / c);
			float flDamage = a * exp(-d * d);

			damage = float((std::max)((int)ceilf(CSGO_Armor(flDamage, local->ArmorValue())), 0));

			sprintf_s(bombdamagestringdead, sizeof(bombdamagestringdead) - 1, "Health Left: 0");
			sprintf_s(bombdamagestringalive, sizeof(bombdamagestringalive) - 1, "Health Left: %.0f", local->GetHealth() - damage);
			if (lifetime > -2.f)
			{
				if (damage >= local->GetHealth())
				{
					g_Render->Text((int)vScreen.x, int(vScreen.y + 10), Color(250, 42, 42, 255), g_Render->font.ESP, bombdamagestringdead);
				}
				else if (local->GetHealth() > damage)
				{
					g_Render->Text((int)vScreen.x, int(vScreen.y + 10), Color(0, 255, 0, 255), g_Render->font.ESP, bombdamagestringalive);
				}
			}
		}
		char buffer[64];
		if (lifetime > 0.01f && !Bomb->IsBombDefused())
		{
			sprintf_s(buffer, "Bomb: %.1f", lifetime);
			g_Render->Text((int)vScreen.x, (int)vScreen.y, Color(250, 42, 42, 255), g_Render->font.ESP, buffer);
		}


	}

	g_Engine->GetScreenSize(width, height);
	int halfX = width / 2;
	int halfY = height / 2;


	if (Bomb->GetBombDefuser() > 0)
	{
		float countdown = Bomb->GetC4DefuseCountDown() - (local->GetTickBase() * g_Globals->interval_per_tick);
		if (countdown > 0.01f)
		{
			if (lifetime > countdown)
			{
				char defuseTimeString[24];
				sprintf_s(defuseTimeString, sizeof(defuseTimeString) - 1, "Defusing: %.1f", countdown);
				g_Render->Text(halfX - 50, halfY + 200, Color(0, 255, 0, 255), g_Render->font.ESP, defuseTimeString);
			}
			else
			{
				g_Render->Text(halfX - 50, halfY + 200, Color(255, 0, 0, 255), g_Render->font.ESP, "No Time! Run!");
			}
		}

	}
}

void visuals::DrawBomb(C_BaseEntity* entity, ClientClass* cClass)
{
	// Null it out incase bomb has been dropped or planted
	BombCarrier = nullptr;
	CBaseCombatWeapon *BombWeapon = (CBaseCombatWeapon *)entity;
	Vector vOrig; Vector vScreen;
	vOrig = entity->GetVecOrigin();
	bool adopted = true;
	auto parent = BombWeapon->GetOwnerHandle();
	if (parent || (vOrig.x == 0 && vOrig.y == 0 && vOrig.z == 0))
	{
		C_BaseEntity* pParentEnt = (g_EntityList->GetClientEntityFromHandle(parent));
		if (pParentEnt && pParentEnt->IsAlive())
		{
			BombCarrier = pParentEnt;
			adopted = false;
		}
	}
	if (g_Options.Visuals.C4World)
	{
		if (adopted)
		{
			if (g_Render->WorldToScreen(vOrig, vScreen))
			{
				g_Render->Text((int)vScreen.x, (int)vScreen.y, Color(255, 20, 20, 255), g_Render->font.ESP, "Bomb");
			}
		}

	}


}

void visuals::Hitbox(int index)
{
	if (g_Options.Visuals.HitboxDuration == 0.f || !g_Options.Visuals.Hitbox)
		return;

	C_BaseEntity* pLocal = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());

	float duration = g_Options.Visuals.HitboxDuration;

	if (index < 0)
		return;

	Color color = Color(g_Options.Colors.DeathHitbox);

	auto entity = reinterpret_cast<C_BaseEntity*>(g_EntityList->GetClientEntity(index));

	if (!entity)
		return;

	studiohdr_t* pStudioModel = g_ModelInfo->GetStudiomodel(entity->GetModel());

	if (!pStudioModel)
		return;

	static matrix3x4 pBoneToWorldOut[128];

	if (!entity->SetupBones(pBoneToWorldOut, MAXSTUDIOBONES, 256, g_Globals->curtime))
		return;

	mstudiohitboxset_t* pHitboxSet = pStudioModel->GetHitboxSet(0);
	if (!pHitboxSet)
		return;

	for (int i = 0; i < pHitboxSet->numhitboxes; i++)
	{
		mstudiobbox_t* pHitbox = pHitboxSet->pHitbox(i);
		if (!pHitbox)
			continue;

		Vector vMin, vMax;
		VectorTransform(pHitbox->bbmin, pBoneToWorldOut[pHitbox->bone], vMin); //nullptr???
		VectorTransform(pHitbox->bbmax, pBoneToWorldOut[pHitbox->bone], vMax);

		if (pHitbox->m_flRadius > -1)
		{
			g_DebugOverlay->AddCapsuleOverlay(vMin, vMax, pHitbox->m_flRadius, color.r(), color.g(), color.b(), 100, duration);
		}
	}
}

void visuals::DrawHealth(RECT rect, C_BaseEntity* entity)
{
	float HealthValue = entity->GetHealth();
	int Red = 255 - (HealthValue * 2.00);
	int Green = HealthValue * 2.00;

	float height = (rect.bottom - rect.top) * (HealthValue / 100);

	g_Render->DrawFilledRect(rect.left - 5, rect.top - 1, rect.left - 1, rect.bottom + 1, Color(0, 0, 0, 110));

	if (entity->IsDormant())
		g_Render->DrawFilledRect(rect.left - 4, rect.bottom - height, rect.left - 2, rect.bottom, Color(114, 114, 114, 114));
	else
		g_Render->DrawFilledRect(rect.left - 4, rect.bottom - height, rect.left - 2, rect.bottom, Color(Red, Green, 0, 255));
}

void visuals::armorbar(RECT rect, C_BaseEntity* entity)
{
	float ArmorValue = entity->ArmorValue();
	int Red = 255 - ArmorValue * 2.00;
	int Green = ArmorValue * 2.00;

	float height = (rect.bottom - rect.top) * (ArmorValue / 100);

	g_Render->DrawFilledRect(rect.left - 5, rect.top - 1, rect.left - 1, rect.bottom + 1, Color(0, 0, 0, 110));

	if (entity->IsDormant())
		g_Render->DrawFilledRect(rect.left - 4, rect.bottom - height, rect.left - 2, rect.bottom, Color(114, 114, 114, 114));
	else
		g_Render->DrawFilledRect(rect.left - 4, rect.bottom - height, rect.left - 2, rect.bottom, Color(Red, Green, 0, 255));
}

void visuals::AmmoBox(RECT rect, C_BaseEntity* entity, Color color)
{
	auto animLayer = &entity->GetAnimOverlays()[1];

	if (!animLayer->m_pOwner)
		return;

	auto activity = entity->GetSequenceActivity(animLayer->m_nSequence);

	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)g_EntityList->GetClientEntityFromHandle(entity->GetActiveWeaponHandle());

	int iClip = pWeapon->LoadedAmmo();
	int iClipMax = pWeapon->GetCSWpnData()->m_iMaxClip();

	float box_w = (rect.right - rect.left);
	float width;
	if (activity == 967 && animLayer->m_flWeight != 0.f)
	{
		float cycle = animLayer->m_flCycle; // 1 = finished 0 = just started
		width = (((box_w * cycle) / 1.f));
	}
	else
		width = (((box_w * iClip) / iClipMax));

	g_Render->DrawFilledRect(rect.left - 1, rect.bottom + 1, rect.right + 1, rect.bottom + 5, Color(0, 0, 0, 110));

	if (entity->IsDormant())
		g_Render->DrawFilledRect(rect.left, rect.bottom + 2, rect.left + width, rect.bottom + 4, Color(114, 114, 114, 114));
	else
		g_Render->DrawFilledRect(rect.left, rect.bottom + 2, rect.left + width, rect.bottom + 4, color);
}

void visuals::DrawBox(RECT rect, Color Col, C_BaseEntity* entity)
{
	g_Render->DrawOutlinedRect(rect.left - 1, rect.top - 1, rect.right - rect.left + 2, rect.bottom - rect.top + 2, Color(0, 0, 0, 110));
	g_Render->DrawOutlinedRect(rect.left + 1, rect.top + 1, rect.right - rect.left - 2, rect.bottom - rect.top - 2, Color(0, 0, 0, 110));
	g_Render->DrawOutlinedRect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, Col);
}

void visuals::RenderFill(RECT rect)
{
	g_Surface->DrawFilledRect(rect.left + 2, rect.top + 2, rect.right - 2, rect.bottom - 2);
}

void visuals::DrawCorners(RECT rect, Color Col, C_BaseEntity* entity)
{
	int x1, y1, x2, y2, w, h;
	x1 = rect.left;
	y1 = rect.top;
	x2 = rect.right;
	y2 = rect.bottom;

	w = x2;
	h = y2;

	int Line_Size = (y1 - h) / 6;
	int Line_Size2 = (y1 - h) / 6;

	int r = 0;
	int g = 0;
	int b = 0;
	int a = 0;
	Col.GetColor(r, g, b, a);
	if (!entity->IsDormant())
		g_Surface->DrawSetColor(r, g, b, a);
	else
		g_Surface->DrawSetColor(box);
	

	//top inwards
	g_Surface->DrawLine(w, y1, w + Line_Size, y1);
	g_Surface->DrawLine(x1, y1, x1 - Line_Size, y1);

	//top downwards
	g_Surface->DrawLine(x1, y1, x1, y1 - Line_Size);
	g_Surface->DrawLine(w, y1, w, y1 - Line_Size);

	//bottom inwards
	g_Surface->DrawLine(x1, h, x1 - Line_Size, h);
	g_Surface->DrawLine(w, h, w + Line_Size, h);

	//bottom upwards
	g_Surface->DrawLine(x1, h, x1, h + Line_Size);
	g_Surface->DrawLine(w, h, w, h + Line_Size);

	//outlines

	g_Surface->DrawSetColor(0, 0, 0, 200);

	//top inwards
	g_Surface->DrawLine(w, y1 - 1, w + Line_Size, y1 - 1);
	g_Surface->DrawLine(x1, y1 - 1, x1 - Line_Size, y1 - 1);
	//inlines
	g_Surface->DrawLine(w - 1, y1 + 1, w + Line_Size, y1 + 1);
	g_Surface->DrawLine(x1 + 1, y1 + 1, x1 - Line_Size, y1 + 1);

	// top downwards
	g_Surface->DrawLine(x1 - 1, y1 - 1, x1 - 1, y1 - Line_Size);
	g_Surface->DrawLine(w + 1, y1 - 1, w + 1, y1 - Line_Size);
	//inlines
	g_Surface->DrawLine(x1 + 1, y1, x1 + 1, y1 - Line_Size);
	g_Surface->DrawLine(w - 1, y1, w - 1, y1 - Line_Size);

	//bottom inwards
	g_Surface->DrawLine(x1, h + 1, x1 - Line_Size, h + 1);
	g_Surface->DrawLine(w, h + 1, w + Line_Size, h + 1);
	//inlines
	g_Surface->DrawLine(x1 + 1, h - 1, x1 - Line_Size, h - 1);
	g_Surface->DrawLine(w - 1, h - 1, w + Line_Size, h - 1);

	//bottom upwards
	g_Surface->DrawLine(x1 - 1, h + 1, x1 - 1, h + Line_Size);
	g_Surface->DrawLine(w + 1, h + 1, w + 1, h + Line_Size);
	//inlines
	g_Surface->DrawLine(x1 + 1, h, x1 + 1, h + Line_Size);
	g_Surface->DrawLine(w - 1, h, w - 1, h + Line_Size);
}

void visuals::ThreeDBox(Vector minin, Vector maxin, Vector pos, Color Col)
{
	Vector min = minin + pos;
	Vector max = maxin + pos;

	Vector corners[] = { Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(min.x, min.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(max.x, max.y, max.z),
		Vector(max.x, min.y, max.z) };

	int edges[12][2] = { { 0, 1 },{ 1, 2 },{ 2, 3 },{ 3, 0 },{ 4, 5 },{ 5, 6 },{ 6, 7 },{ 7, 4 },{ 0, 4 },{ 1, 5 },{ 2, 6 },{ 3, 7 }, };

	for (const auto edge : edges)
	{
		Vector p1, p2;
		if (!g_Render->WorldToScreen(corners[edge[0]], p1) || !g_Render->WorldToScreen(corners[edge[1]], p2))
			return;
		int red = 0;
		int green = 0;
		int blue = 0;
		int alpha = 0;
		Col.GetColor(red, green, blue, alpha);
		g_Surface->DrawSetColor(red, green, blue, alpha);
		g_Surface->DrawLine(p1.x, p1.y, p2.x, p2.y);
	}

}

void visuals::OffscreenArrow(C_BaseEntity* entity)
{
	if (!g_Options.Visuals.OffscreenIndicator)
		return;

	C_BaseEntity* local = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());

	if (!local)
		return;

	static auto a = 0.f;
	static auto plus_or_minus = false;
	if (a <= 0.f || a >= 255.f)
		plus_or_minus = !plus_or_minus;

	a += plus_or_minus ? (255.f / 160.f * g_Globals->frametime) : -(255.f / 160.f * g_Globals->frametime);
	a = clamp<float>(a, 0.f, 255.f);

	Color color;

	Vector pos, pos3D;

	ClientClass* cClass = (ClientClass*)entity->GetClientClass();

	if (entity->IsAlive() && cClass->m_ClassID == (int)ClassID::CCSPlayer)
	{
		if (entity->IsDormant())
			color = Color(143, 143, 143, a);
		else
			color = Color(g_Options.Colors.OffscreenIndicator);
		
		pos3D = entity->GetVecOrigin();

		switch (g_Options.Visuals.OffscreenIndicatorCheck)
		{
		case 1://WorldToScreen
			if (g_Render->WorldToScreen(pos3D, pos))
				return;
			break;
		case 2://Visible
			if (visuals::IsVisibleScan(entity) == true)
				return;
			break;
		case 3://Smart | Both
			if (visuals::IsVisibleScan(entity) == true && g_Render->WorldToScreen(pos3D, pos))
				return;
			break;
		}

		auto pLocal = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());


		Vector screenPos;

		auto client_viewangles = Vector();
		auto screen_width = 0, screen_height = 0;

		g_Engine->GetViewAngles(client_viewangles);
		g_Engine->GetScreenSize(screen_width, screen_height);

		auto radius = g_Options.Visuals.OffscreenIndicatorRadius;

		Vector local_position = pLocal->GetVecOrigin() + pLocal->GetViewOffset();

		const auto screen_center = Vector(screen_width / 2.f, screen_height / 2.f, 0);
		const auto rot = DEG2RAD(client_viewangles.y - CalcAngle(local_position, GetHitboxPosition(entity, Stomach)).y - 90);

		std::vector<Vertex_t> vertices;

		vertices.push_back(Vertex_t(Vector2D(screen_center.x + cosf(rot) * radius, screen_center.y + sinf(rot) * radius)));
		vertices.push_back(Vertex_t(Vector2D(screen_center.x + cosf(rot + DEG2RAD(2)) * (radius - g_Options.Visuals.OffscreenIndicatorSize), screen_center.y + sinf(rot + DEG2RAD(2)) * (radius - g_Options.Visuals.OffscreenIndicatorSize))));//16
		vertices.push_back(Vertex_t(Vector2D(screen_center.x + cosf(rot - DEG2RAD(2)) * (radius - g_Options.Visuals.OffscreenIndicatorSize), screen_center.y + sinf(rot - DEG2RAD(2)) * (radius - g_Options.Visuals.OffscreenIndicatorSize))));


		if (pLocal->GetTeamNum() != entity->GetTeamNum())
		{
			g_Render->TexturedPolygon(3, vertices, color);
		}

		else if (pLocal->GetTeamNum() == entity->GetTeamNum() && g_Options.Visuals.TeamESP)
		{
			g_Render->TexturedPolygon(3, vertices, Color(0, 255, 0, 165));
		}
	}
}

void visuals::DrawInfo(RECT rect, C_BaseEntity* entity, C_BaseEntity* local)
{
	player_info_t info;
	static class Text
	{
	public:
		std::string text;
		int side;
		int Font;
		Color color;

		Text(std::string text, int side, int Font, Color color) : text(text), side(side), Font(Font), color(color)
		{
		}
	};
	std::vector< Text > texts;

	CBaseCombatWeapon* weapon = (CBaseCombatWeapon*)g_EntityList->GetClientEntityFromHandle(entity->GetActiveWeaponHandle());

	if (g_Engine->GetPlayerInfo(entity->GetIndex(), &info))
	{
		const char* resolverMode = "NONE";

		if (Globals::resolverModes[entity->GetIndex()] == 1)
			resolverMode = "OVERRIDE";

		if (Globals::resolverModes[entity->GetIndex()] == 2)
			resolverMode = "FAKEWALK";

		if (Globals::resolverModes[entity->GetIndex()] == 3)
			resolverMode = "MOVING";

		if (Globals::resolverModes[entity->GetIndex()] == 4)
			resolverMode = "FAKE (LBY > 35)";

		if (Globals::resolverModes[entity->GetIndex()] == 5)
			resolverMode = "BREAKING LBY";

		if (Globals::resolverModes[entity->GetIndex()] == 6)
			resolverMode = "BREAKING LBY (LBY < 120)";

		if (Globals::resolverModes[entity->GetIndex()] == 7)
			resolverMode = "LBY < 35";

		if (Globals::resolverModes[entity->GetIndex()] == 8)
			resolverMode = "OTHER";



		if (!entity->IsDormant())
		{

			if (g_Options.Visuals.armorflag)
				texts.push_back(Text(entity->GetArmorName(), 1, g_Render->font.ESP, Color(50, 96, 148, 255)));

			if (g_Options.Visuals.Money)
				texts.push_back(Text(std::string("$") + std::to_string(entity->iAccount()), 1, g_Render->font.ESP, Color(39, 229, 0)));

			if (g_Options.Visuals.Scoped && entity->IsScoped())
				texts.push_back(Text("ZOOM", 1, g_Render->font.ESP, Color(153, 147, 220)));

			if (g_Options.Visuals.Flashed && entity->IsFlashed())
				texts.push_back(Text("BLIND", 1, g_Render->font.ESP, Color(224, 224, 224)));

			if (g_Options.Visuals.IsHasDefuser && entity->hasDefuser())
				texts.push_back(Text("KIT", 1, g_Render->font.ESP, Color(14, 116, 239)));

			if (g_Options.Visuals.IsDefusing && entity->IsDefusing())
				texts.push_back(Text("DEFUSE", 1, g_Render->font.ESP, Color(2, 139, 237)));

			if (g_Options.Visuals.BombCarrier &&  g_Options.Visuals.Enabled && entity == BombCarrier)
				texts.push_back(Text("BOMB", 1, g_Render->font.ESP, Color(216, 23, 23)));

			if (g_Options.Visuals.Name)
				texts.push_back(Text(info.name, 0, g_Render->font.Name, Color(224, 224, 224)));

			if (weapon)
			{
				switch (g_Options.Visuals.Weapon)
				{
				case 0:
					break;
				case 1:
					if (MiscFunctions::IsKnife(weapon) || MiscFunctions::IsGrenade(weapon) || MiscFunctions::IsZeus(weapon))
						texts.push_back(Text(weapon->GetGunName(), 2, g_Render->font.ESP, Color(224, 224, 224)));
					else
						texts.push_back(Text(weapon->GetGunName() + std::string(" [") + std::to_string(weapon->LoadedAmmo()) + std::string(" | ") + std::to_string(weapon->ReserveAmmo()) + std::string("]"), 2, g_Render->font.ESP, Color(224, 224, 224)));
					break;
				case 2:
					texts.push_back(Text(weapon->GetGunIcon(), 2, g_Render->font.Icon, Color(224, 224, 224)));
					break;
				default:
					break;
				}
			}

			if (g_Options.Visuals.Distance)
				texts.push_back(Text(std::to_string(flGetDistance(local->GetVecOrigin(), entity->GetVecOrigin())) + std::string("FT"), 2, g_Render->font.ESP, Color(224, 224, 224)));

			if (g_Options.Visuals.resolveMode && g_Options.Ragebot.Resolver)
				texts.push_back(Text(resolverMode, 2, g_Render->font.ESP, Color(224, 224, 224)));

			//		if (g_Options.Visuals.Reloading && weapon->IsInReload())
			//		{
			//			texts.push_back(Text("*Reloading*", 0, g_Render->font.ESP, Color(252, 147, 0)));
			//		}
		}
		else
		{
			if (g_Options.Visuals.armorflag)
				texts.push_back(Text(entity->GetArmorName(), 1, g_Render->font.ESP, Color(114, 114, 114, 144)));

			if (g_Options.Visuals.Money)
				texts.push_back(Text(std::string("$") + std::to_string(entity->iAccount()), 1, g_Render->font.ESP, Color(114, 114, 114, 144)));

			if (g_Options.Visuals.Scoped && entity->IsScoped())
				texts.push_back(Text("ZOOM", 1, g_Render->font.ESP, Color(114, 114, 114, 144)));

			if (g_Options.Visuals.Flashed && entity->IsFlashed())
				texts.push_back(Text("BLIND", 1, g_Render->font.ESP, Color(114, 114, 114, 144)));

			if (g_Options.Visuals.IsHasDefuser && entity->hasDefuser())
				texts.push_back(Text("KIT", 1, g_Render->font.ESP, Color(114, 114, 114, 144)));

			if (g_Options.Visuals.IsDefusing && entity->IsDefusing())
				texts.push_back(Text("DEFUSE", 1, g_Render->font.ESP, Color(114, 114, 114, 144)));

			if (g_Options.Visuals.BombCarrier &&  g_Options.Visuals.Enabled && entity == BombCarrier)
				texts.push_back(Text("BOMB", 1, g_Render->font.ESP, Color(114, 114, 114, 144)));

			if (g_Options.Visuals.Name)
				texts.push_back(Text(info.name, 0, g_Render->font.Name, Color(114, 114, 114, 144)));

			if (weapon)
			{
				switch (g_Options.Visuals.Weapon)
				{
				case 0:
					break;
				case 1:
					if (MiscFunctions::IsKnife(weapon) || MiscFunctions::IsGrenade(weapon) || MiscFunctions::IsZeus(weapon))
						texts.push_back(Text(weapon->GetGunName(), 2, g_Render->font.ESP, Color(114, 114, 114, 144)));
					else
						texts.push_back(Text(weapon->GetGunName() + std::string(" [") + std::to_string(weapon->LoadedAmmo()) + std::string(" | ") + std::to_string(weapon->ReserveAmmo()) + std::string("]"), 2, g_Render->font.ESP, Color(114, 114, 114, 144)));
					break;
				case 2:
					texts.push_back(Text(weapon->GetGunIcon(), 2, g_Render->font.Icon, Color(114, 114, 114, 144)));
					break;
				default:
					break;
				}
			}

			if (g_Options.Visuals.Distance)
				texts.push_back(Text(std::to_string(flGetDistance(local->GetVecOrigin(), entity->GetVecOrigin())) + std::string("FT"), 2, g_Render->font.ESP, Color(114, 114, 114, 144)));

			if (g_Options.Visuals.resolveMode && g_Options.Ragebot.Resolver)
				texts.push_back(Text(resolverMode, 2, g_Render->font.ESP, Color(114, 114, 114, 114)));

			//		if (g_Options.Visuals.Reloading && weapon->IsInReload())
			//		{
			//			texts.push_back(Text("*Reloading*", 0, g_Render->font.ESP, Color(114, 114, 114, 144)));
			//		}
		}

	}


	int middle = ((rect.right - rect.left) / 2) + rect.left;
	int Top[3] = { rect.top,rect.top, rect.bottom };
	for (auto text : texts)
	{
		RECT nameSize = g_Render->GetTextSize(text.Font, (char*)text.text.c_str());
		switch (text.side)
		{
		case 0:
			Top[0] -= nameSize.bottom + 1;
			g_Render->DrawString2(text.Font, middle, Top[0] + 7, text.color, FONT_CENTER, (char*)text.text.c_str());//top
			break;
		case 1:
			g_Render->DrawString2(text.Font, rect.right + 2, Top[1] + 4, text.color, FONT_LEFT, (char*)text.text.c_str());//right
			Top[1] += nameSize.bottom - 4;
			break;
		case 2:
			g_Render->DrawString2(text.Font, middle, Top[2] + 12, text.color, FONT_CENTER, (char*)text.text.c_str());//bottom
			Top[2] += nameSize.bottom - 4;
			break;
		}
	}

}

void visuals::DrawArrows(C_BaseEntity* local)
{
	int screen_width, screen_height;
	g_Engine->GetScreenSize(screen_width, screen_height);

	if (!local || !local->IsAlive())
		return;

	if (!g_Options.Visuals.angleArrows || !g_Options.Ragebot.EnabledAntiAim)
		return;

	if (g_Input->m_fCameraInThirdPerson)
		return;

	static const auto fake_color = Color(255, 0, 0, 70);


	auto client_viewangles = Vector();
	g_Engine->GetViewAngles(client_viewangles);

	constexpr auto radius = 90.f;

	const auto screen_center = Vector2D(screen_width / 2.f, screen_height / 2.f);
	const auto fake_rot = DEG2RAD(client_viewangles.y - Globals::FakeAngle - 90);

	auto draw_arrow = [&](float rot, Color color) -> void
	{
		std::vector<Vertex_t> vertices;
		vertices.push_back(Vertex_t(Vector2D(screen_center.x + cosf(rot) * radius, screen_center.y + sinf(rot) * radius)));
		vertices.push_back(Vertex_t(Vector2D(screen_center.x + cosf(rot + DEG2RAD(4)) * (radius - 14.f), screen_center.y + sinf(rot + DEG2RAD(4)) * (radius - 14.f)))); //25
		vertices.push_back(Vertex_t(Vector2D(screen_center.x + cosf(rot - DEG2RAD(4)) * (radius - 14.f), screen_center.y + sinf(rot - DEG2RAD(4)) * (radius - 14.f)))); //25
		g_Render->TexturedPolygon(3, vertices, color);
	};

	draw_arrow(fake_rot, fake_color);

	static const auto real_color = Color(0, 255, 0, 70);

	const auto real_rot = DEG2RAD(client_viewangles.y - Globals::RealAngle - 90);

	draw_arrow(real_rot, real_color);

	static const auto lby_color = Color(0, 0, 255, 70);

	const auto lby_rot = DEG2RAD(client_viewangles.y - local->GetLowerBodyYaw() - 90);

	draw_arrow(lby_rot, lby_color);
}

void visuals::DrawAngles()
{
	C_BaseEntity *pLocal = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());

	Vector src3D, dst3D, forward, src, dst;
	trace_t tr;
	Ray_t ray;
	CTraceFilter filter;

	filter.pSkip = pLocal;
	src3D = pLocal->GetVecOrigin();

	AngleVectors(QAngle(0, Globals::RealAngle, 0), &forward);
	dst3D = src3D + (forward * 45.f);

	ray.Init(src3D, dst3D);

	g_EngineTrace->TraceRay(ray, 0, &filter, &tr);

	if (!g_Render->WorldToScreen(src3D, src) || !g_Render->WorldToScreen(tr.endpos, dst))
		return;

	g_Render->Line(src.x, src.y, dst.x, dst.y, Color(0, 255, 0, 255));

	if (g_Options.Visuals.angleLinesName)
		g_Render->Text(dst.x, dst.y, Color(0, 255, 0, 255), g_Render->font.ESP, "REAL");

	AngleVectors(QAngle(0, Globals::FakeAngle, 0), &forward);
	dst3D = src3D + (forward * 45.f);

	ray.Init(src3D, dst3D);

	g_EngineTrace->TraceRay(ray, 0, &filter, &tr);

	if (!g_Render->WorldToScreen(src3D, src) || !g_Render->WorldToScreen(tr.endpos, dst))
		return;

	if (g_Options.Ragebot.EnabledAntiAim)
	{
		g_Render->Line(src.x, src.y, dst.x, dst.y, Color(255, 0, 0, 255));

		if (g_Options.Visuals.angleLinesName)
			g_Render->Text(dst.x, dst.y, Color(255, 0, 0, 255), g_Render->font.ESP, "FAKE");
	}

	AngleVectors(QAngle(0, pLocal->GetLowerBodyYaw(), 0), &forward);
	dst3D = src3D + (forward * 45.f);

	ray.Init(src3D, dst3D);

	g_EngineTrace->TraceRay(ray, 0, &filter, &tr);

	if (!g_Render->WorldToScreen(src3D, src) || !g_Render->WorldToScreen(tr.endpos, dst))
		return;

	g_Render->Line(src.x, src.y, dst.x, dst.y, Color(255, 127, 0, 255));

	if (g_Options.Visuals.angleLinesName)
		g_Render->Text(dst.x, dst.y, Color(255, 0, 0, 255), g_Render->font.ESP, "LBY");

}

RECT visuals::DynamicBox(C_BaseEntity* pPlayer, bool& PVS, C_BaseEntity* local)
{
	Vector trans = pPlayer->GetVecOrigin();

	Vector min;
	Vector max;

	min = pPlayer->GetCollideable()->OBBMins();
	max = pPlayer->GetCollideable()->OBBMaxs();

	Vector pointList[] = {
		Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z),
		Vector(max.x, min.y, max.z)
	};

	Vector Distance = pointList[0] - pointList[1];
	int dst = Distance.Length();
	dst /= 1.3f;
	Vector angs;
	CalcAngle(trans, local->GetEyePosition(), angs);

	Vector all[8];
	angs.y += 45;
	for (int i = 0; i < 4; i++)
	{
		AngleVectors(angs, &all[i]);
		all[i] *= dst;
		all[i + 4] = all[i];
		all[i].z = max.z;
		all[i + 4].z = min.z;
		VectorAdd(all[i], trans, all[i]);
		VectorAdd(all[i + 4], trans, all[i + 4]);
		angs.y += 90;
	}

	Vector flb, brt, blb, frt, frb, brb, blt, flt;
	PVS = true;

	if (!g_DebugOverlay->ScreenPosition(all[3], flb))
		PVS = false;
	if (!g_DebugOverlay->ScreenPosition(all[0], blb))
		PVS = false;
	if (!g_DebugOverlay->ScreenPosition(all[2], frb))
		PVS = false;
	if (!g_DebugOverlay->ScreenPosition(all[6], blt))
		PVS = false;
	if (!g_DebugOverlay->ScreenPosition(all[5], brt))
		PVS = false;
	if (!g_DebugOverlay->ScreenPosition(all[4], frt))
		PVS = false;
	if (!g_DebugOverlay->ScreenPosition(all[1], brb))
		PVS = false;
	if (!g_DebugOverlay->ScreenPosition(all[7], flt))
		PVS = false;

	Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	float left = flb.x;
	float top = flb.y;
	float right = flb.x;
	float bottom = flb.y;

	for (int i = 0; i < 8; i++)
	{
		if (left > arr[i].x)
			left = arr[i].x;
		if (top > arr[i].y)
			top = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (bottom < arr[i].y)
			bottom = arr[i].y;
	}
	RECT rect;
	rect.left = left;
	rect.bottom = bottom;
	rect.right = right;
	rect.top = top;

	return rect;
}

bool visuals::GetBBox(C_BaseEntity* entity, visuals::ESPBox &result)
{

	// Variables
	Vector  vOrigin, min, max, sMin, sMax, sOrigin,
		flb, brt, blb, frt, frb, brb, blt, flt;
	float left, top, right, bottom;


	vOrigin = entity->GetAbsOrigin2();
	min = entity->collisionProperty()->GetMins() + vOrigin;
	max = entity->collisionProperty()->GetMaxs() + vOrigin;

	// Points of a 3d bounding box
	Vector points[] = { Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z),
		Vector(max.x, min.y, max.z) };

	// Get screen positions
	if (!g_Render->WorldToScreen(points[3], flb) || !g_Render->WorldToScreen(points[5], brt)
		|| !g_Render->WorldToScreen(points[0], blb) || !g_Render->WorldToScreen(points[4], frt)
		|| !g_Render->WorldToScreen(points[2], frb) || !g_Render->WorldToScreen(points[1], brb)
		|| !g_Render->WorldToScreen(points[6], blt) || !g_Render->WorldToScreen(points[7], flt))
		return false;

	// Put them in an array (maybe start them off in one later for speed?)
	Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	// Init this shit
	left = flb.x;
	top = flb.y;
	right = flb.x;
	bottom = flb.y;

	// Find the bounding corners for our box
	for (int i = 1; i < 8; i++)
	{
		if (left > arr[i].x)
			left = arr[i].x;
		if (bottom < arr[i].y)
			bottom = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (top > arr[i].y)
			top = arr[i].y;
	}
	RECT rect;
	rect.left = left;
	rect.top = top;
	rect.right = right;
	rect.bottom = bottom;

	// Width / height
	result.x = left;
	result.y = top;
	result.w = right - left;
	result.h = bottom - top;
	return true;
}

void visuals::RenderWeapon(C_BaseEntity* entity, Color color)
{

	if (entity)
	{
		CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)entity;

		auto owner = pWeapon->GetOwnerHandle();

		if (owner > -1)
			return;

		Vector pos3D = entity->GetAbsOrigin2();

		if (pos3D.x == 0.0f && pos3D.y == 0.0f && pos3D.z == 0.0f)
			return;

		Vector pos;


		if (!g_Render->WorldToScreen(pos3D, pos))
			return;

		auto weaponID = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();
		auto weaponName = ItemDefinitionIndexxToString(weaponID);
		auto weaponIcon = ItemDefinitionIndexxToString2(weaponID);


		switch (g_Options.Visuals.DroppedGunsType)
		{
		case 0:
			break;
		case 1:
			g_Render->Text(pos.x, pos.y, color, g_Render->font.Guns, weaponName);
			break;
		case 2:
			g_Render->Text(pos.x, pos.y, color, g_Render->font.Icon, weaponIcon);
			break;
		default:
			break;
		}

	}
}
void ColorConvertHSVtoRGB(float h, float s, float v, float& out_r, float& out_g, float& out_b)
{
	if (s == 0.0f)
	{
		// gray
		out_r = out_g = out_b = v;
		return;
	}

	h = fmodf(h, 1.0f) / (60.0f / 360.0f);
	int   i = (int)h;
	float f = h - (float)i;
	float p = v * (1.0f - s);
	float q = v * (1.0f - s * f);
	float t = v * (1.0f - s * (1.0f - f));

	switch (i)
	{
	case 0: out_r = v; out_g = t; out_b = p; break;
	case 1: out_r = q; out_g = v; out_b = p; break;
	case 2: out_r = p; out_g = v; out_b = t; break;
	case 3: out_r = p; out_g = q; out_b = v; break;
	case 4: out_r = t; out_g = p; out_b = v; break;
	case 5: default: out_r = v; out_g = p; out_b = q; break;
	}
}
void visuals::DrawTaserRange()
{
	Vector prev_scr_pos, scr_pos;
	C_BaseEntity* local = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
	CBaseCombatWeapon* weapon = (CBaseCombatWeapon*)g_EntityList->GetClientEntityFromHandle(local->GetActiveWeaponHandle());
	if (!weapon || !local || !MiscFunctions::IsZeus(weapon))
		return;

	float step = M_PI * 2.0 / 150; // / 75
	float rad = weapon->GetCSWpnData()->m_fRange();
	Vector origin = local->GetEyePosition();

	for (float rotation = 0; rotation < (M_PI * 2.0); rotation += step)
	{
		Vector pos(rad * cos(rotation) + origin.x, rad * sin(rotation) + origin.y, origin.z);

		Ray_t ray;
		trace_t trace;
		CTraceFilter filter;

		filter.pSkip = local;
		ray.Init(origin, pos);

		g_EngineTrace->TraceRay(ray, MASK_SHOT_BRUSHONLY, &filter, &trace);

		if (g_Render->WorldToScreen(trace.endpos, scr_pos))
		{
			if (prev_scr_pos.IsValid())
			{
				Color color = Color(255, 0, 0);

				g_Surface->DrawSetColor(color);
				g_Surface->DrawLine(prev_scr_pos.x, prev_scr_pos.y, scr_pos.x, scr_pos.y);
			}
			prev_scr_pos = scr_pos;
		}
	}
}

void visuals::BulletTrace(C_BaseEntity* pEntity, Color color)
{
	Vector src3D, dst3D, forward, src, dst;
	trace_t tr;
	Ray_t ray;
	CTraceFilter filter;
	Vector eyes = *pEntity->GetEyeAngles();

	AngleVectors(eyes, &forward);
	filter.pSkip = pEntity;
	src3D = pEntity->GetBonePos(8) - Vector(0, 0, 0);
	dst3D = src3D + (forward * g_Options.Visuals.barrelL);

	ray.Init(src3D, dst3D);

	g_EngineTrace->TraceRay(ray, MASK_SHOT, &filter, &tr);

	if (!g_Render->WorldToScreen(src3D, src) || !g_Render->WorldToScreen(tr.endpos, dst))
		return;


	g_Render->Line(src.x, src.y, dst.x, dst.y, color);
	g_Render->DrawOutlinedRect(dst.x - 3, dst.y - 3, 6, 6, color);

};

void visuals::Skeleton(C_BaseEntity* pEntity, Color Col)
{
	studiohdr_t* pStudioHdr = g_ModelInfo->GetStudiomodel(pEntity->GetModel());

	if (!pStudioHdr)
		return;

	Vector vParent, vChild, sParent, sChild;

	for (int j = 0; j < pStudioHdr->numbones; j++)
	{
		mstudiobone_t* pBone = pStudioHdr->GetBone(j);

		if (pBone && pBone->flags & BONE_USED_BY_Hitbox && (pBone->parent != -1))
		{
			vChild = pEntity->GetBonePos(j);
			vParent = pEntity->GetBonePos(pBone->parent);

			int iChestBone = 6;  // Parameter of relevant Bone number
			Vector vBreastBone; // New reference Point for connecting many bones
			Vector vUpperDirection = pEntity->GetBonePos(iChestBone + 1) - pEntity->GetBonePos(iChestBone); // direction vector from chest to neck
			vBreastBone = pEntity->GetBonePos(iChestBone) + vUpperDirection / 2;
			Vector vDeltaChild = vChild - vBreastBone; // Used to determine close bones to the reference point
			Vector vDeltaParent = vParent - vBreastBone;

			// Eliminating / Converting all disturbing bone positions in three steps.
			if ((vDeltaParent.Length() < 9 && vDeltaChild.Length() < 9))
				vParent = vBreastBone;

			if (j == iChestBone - 1)
				vChild = vBreastBone;

			if (abs(vDeltaChild.z) < 5 && (vDeltaParent.Length() < 5 && vDeltaChild.Length() < 5) || j == iChestBone)
				continue;

			g_DebugOverlay->ScreenPosition(vParent, sParent);
			g_DebugOverlay->ScreenPosition(vChild, sChild);

			if (!pEntity->IsDormant())
				g_Surface->DrawSetColor(g_Options.Colors.Skeleton);
			else
				g_Surface->DrawSetColor(Color(114, 114, 114, 114));

			g_Surface->DrawLine(sParent[0], sParent[1], sChild[0], sChild[1]);
		}
	}
}

void visuals::HealthSkeleton(C_BaseEntity* pEntity)
{

	studiohdr_t* pStudioHdr = g_ModelInfo->GetStudiomodel(pEntity->GetModel());

	if (!pStudioHdr)
		return;

	Vector vParent, vChild, sParent, sChild;

	for (int j = 0; j < pStudioHdr->numbones; j++)
	{
		mstudiobone_t* pBone = pStudioHdr->GetBone(j);


		if (pBone && (pBone->flags & BONE_USED_BY_Hitbox) && (pBone->parent != -1))
		{
			vChild = pEntity->GetBonePos(j);
			vParent = pEntity->GetBonePos(pBone->parent);

			int iChestBone = 6;  // Parameter of relevant Bone number
			Vector vBreastBone; // New reference Point for connecting many bones
			Vector vUpperDirection = pEntity->GetBonePos(iChestBone + 1) - pEntity->GetBonePos(iChestBone); // direction vector from chest to neck
			vBreastBone = pEntity->GetBonePos(iChestBone) + vUpperDirection / 2;
			Vector vDeltaChild = vChild - vBreastBone; // Used to determine close bones to the reference point
			Vector vDeltaParent = vParent - vBreastBone;

			// Eliminating / Converting all disturbing bone positions in three steps.
			if ((vDeltaParent.Length() < 9 && vDeltaChild.Length() < 9))
				vParent = vBreastBone;

			if (j == iChestBone - 1)
				vChild = vBreastBone;

			if (abs(vDeltaChild.z) < 5 && (vDeltaParent.Length() < 5 && vDeltaChild.Length() < 5) || j == iChestBone)
				continue;

			g_DebugOverlay->ScreenPosition(vParent, sParent);
			g_DebugOverlay->ScreenPosition(vChild, sChild);

			if (!pEntity->IsDormant())
			{
				if (pEntity->GetHealth() == 100)
					g_Render->Line(sParent[0], sParent[1], sChild[0], sChild[1], Color(0, 255, 0, 255));

				else if (pEntity->GetHealth() < 100 & pEntity->GetHealth() >= 85)
					g_Render->Line(sParent[0], sParent[1], sChild[0], sChild[1], Color(114, 255, 0, 255));

				else if (pEntity->GetHealth() < 85 & pEntity->GetHealth() >= 60)
					g_Render->Line(sParent[0], sParent[1], sChild[0], sChild[1], Color(178, 255, 0, 255));

				else if (pEntity->GetHealth() < 60 & pEntity->GetHealth() >= 45)
					g_Render->Line(sParent[0], sParent[1], sChild[0], sChild[1], Color(255, 229, 0, 255));

				else if (pEntity->GetHealth() < 45 & pEntity->GetHealth() >= 30)
					g_Render->Line(sParent[0], sParent[1], sChild[0], sChild[1], Color(255, 127, 0, 255));

				else if (pEntity->GetHealth() < 30 & pEntity->GetHealth() >= 15)
					g_Render->Line(sParent[0], sParent[1], sChild[0], sChild[1], Color(255, 55, 0, 255));

				else if (pEntity->GetHealth() < 15 & pEntity->GetHealth() > 0)
					g_Render->Line(sParent[0], sParent[1], sChild[0], sChild[1], Color(191, 0, 0, 255));
			}
			else
				g_Render->Line(sParent[0], sParent[1], sChild[0], sChild[1], Color(114, 114, 114, 114));



			
		}
	}
}

void visuals::DrawSnapLine(RECT rect, C_BaseEntity* entity)
{
	Color color;

	if (entity->IsDormant())
		color = snapline;
	else
		color = Color(g_Options.Colors.AimLineColor);

	int width, height;
	g_Engine->GetScreenSize(width, height);

	int screen_x = width * 0.5f,
		screen_y = height,
		target_x = rect.left + (rect.right - rect.left) * 0.5,
		target_y = rect.bottom,
		max_length = height * 0.3f;

	if (target_x == 0 ||
		target_y == 0)
		return;

	float length = sqrt(pow(target_x - screen_x, 2) + pow(target_y - screen_y, 2));
	if (length > max_length)
	{
		float
			x_normalized = (target_x - screen_x) / length,
			y_normalized = (target_y - screen_y) / length;
		target_x = screen_x + x_normalized * max_length;
		target_y = screen_y + y_normalized * max_length;
		g_Render->DrawFilledCircle(Vector2D(target_x + x_normalized * 3.5f, target_y + y_normalized * 3.5f), color, 9.f, 100);
	}

	g_Surface->DrawSetColor(color);
	g_Surface->DrawLine(screen_x, screen_y, target_x, target_y);
}

void visuals::Hitmarker()
{
	Color hitmarkercolor = Color(int(g_Options.Colors.Hitmarker[0] * 255.f), int(g_Options.Colors.Hitmarker[1] * 255.f), int(g_Options.Colors.Hitmarker[2] * 255.f), (G::hitmarkeralpha * 255.f));
	if (G::hitmarkeralpha < 0.f)
		G::hitmarkeralpha = 0.f;
	else if (G::hitmarkeralpha > 0.f)
		G::hitmarkeralpha -= 0.01f;

	int W, H;
	g_Engine->GetScreenSize(W, H);

	if (G::hitmarkeralpha > 0.f)
	{
		g_Render->Line(W / 2 - 6, H / 2 - 6, W / 2 - 3, H / 2 - 3, hitmarkercolor);
		g_Render->Line(W / 2 - 6, H / 2 + 6, W / 2 - 3, H / 2 + 3, hitmarkercolor);
		g_Render->Line(W / 2 + 6, H / 2 - 6, W / 2 + 3, H / 2 - 3, hitmarkercolor);
		g_Render->Line(W / 2 + 6, H / 2 + 6, W / 2 + 3, H / 2 + 3, hitmarkercolor);

	}
}

void visuals::SkyChanger()
{
	static auto sv_skyname = g_CVar->FindVar("sv_skyname");

	switch (g_Options.Visuals.SkyboxChanger)
	{
	case 1: //Baggage
		sv_skyname->SetValue("cs_baggage_skybox_");
		break;
	case 2: //Tibet
		sv_skyname->SetValue("cs_tibet");
		break;
	case 3: //Clear Sky
		sv_skyname->SetValue("clearsky");
		break;
	case 4: //Clear Sky HD
		sv_skyname->SetValue("clearsky_hdr");
		break;
	case 5: //Embassy
		sv_skyname->SetValue("embassy");
		break;
	case 6: //Italy
		sv_skyname->SetValue("italy");
		break;
	case 7: //Daylight 1
		sv_skyname->SetValue("sky_cs15_daylight01_hdr");
		break;
	case 8: //Daylight 2
		sv_skyname->SetValue("sky_cs15_daylight02_hdr");
		break;
	case 9: //Daylight 3
		sv_skyname->SetValue("sky_cs15_daylight03_hdr");
		break;
	case 10: //Daylight 4
		sv_skyname->SetValue("sky_cs15_daylight04_hdr");
		break;
	case 11: //Cloudy
		sv_skyname->SetValue("sky_csgo_cloudy01");
		break;
	case 12: //Night 1
		sv_skyname->SetValue("sky_csgo_night02");
		break;
	case 13: //Night 2
		sv_skyname->SetValue("sky_csgo_night02b");
		break;
	case 14: //Night Flat
		sv_skyname->SetValue("sky_csgo_night_flat");
		break;
	case 15: //Day HD
		sv_skyname->SetValue("sky_day02_05_hdr");
		break;
	case 16: //Day
		sv_skyname->SetValue("sky_day02_05");
		break;
	case 17: //Black
		sv_skyname->SetValue("sky_l4d_rural02_ldr");
		break;
	case 18: //Vertigo HD
		sv_skyname->SetValue("vertigo_hdr");
		break;
	case 19: //Vertigo Blue HD
		sv_skyname->SetValue("vertigoblue_hdr");
		break;
	case 20: //Vertigo
		sv_skyname->SetValue("vertigo");
		break;
	case 21: //Vietnam
		sv_skyname->SetValue("vietnam");
		break;
	case 22: //Dusty Sky
		sv_skyname->SetValue("sky_dust");
		break;
	case 23: //Jungle
		sv_skyname->SetValue("jungle");
		break;
	case 24: //Nuke
		sv_skyname->SetValue("nukeblank");
		break;
	case 25: //Office
		sv_skyname->SetValue("office");
		break;
	}
}

void visuals::NightMode()
{		
	C_BaseEntity* pLocal = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());

	int OldSkyBox = g_Options.Visuals.SkyboxChanger;

	if (g_Options.Visuals.nightMode)
	{
		if (!done)
		{

			g_Options.Visuals.SkyboxChanger = 13;

			for (MaterialHandle_t i = g_MaterialSystem->FirstMaterial(); i != g_MaterialSystem->InvalidMaterial(); i = g_MaterialSystem->NextMaterial(i))
			{
				IMaterial *pMaterial = g_MaterialSystem->GetMaterial(i);

				if (!pMaterial)
					continue;

				const char* group = pMaterial->GetTextureGroupName();
				const char* name = pMaterial->GetName();

				if (strstr(group, "World textures"))
				{
					pMaterial->ColorModulate(0.10, 0.10, 0.10);
				}
				if (strstr(group, "StaticProp"))
				{
					pMaterial->ColorModulate(0.30, 0.30, 0.30);
				}
				if (strstr(name, "models/props/de_dust/palace_bigdome"))
				{
					pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
				}
				if (strstr(name, "models/props/de_dust/palace_pillars"))
				{
					pMaterial->ColorModulate(0.30, 0.30, 0.30);
				}

				if (strstr(group, "Particle textures"))
				{
					pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
				}
				done = true;
			}

		}
	}
	else
	{
		if (done)
		{
			for (MaterialHandle_t i = g_MaterialSystem->FirstMaterial(); i != g_MaterialSystem->InvalidMaterial(); i = g_MaterialSystem->NextMaterial(i))
			{
				
				IMaterial *pMaterial = g_MaterialSystem->GetMaterial(i);

				g_Options.Visuals.SkyboxChanger = OldSkyBox;

				if (!pMaterial)
					continue;

				const char* group = pMaterial->GetTextureGroupName();
				const char* name = pMaterial->GetName();

				if (strstr(group, "World textures"))
				{
					pMaterial->ColorModulate(1, 1, 1);
				}
				if (strstr(group, "StaticProp"))
				{

					pMaterial->ColorModulate(1, 1, 1);
				}
				if (strstr(name, "models/props/de_dust/palace_bigdome"))
				{
					pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
				}
				if (strstr(name, "models/props/de_dust/palace_pillars"))
				{

					pMaterial->ColorModulate(1, 1, 1);
				}
				if (strstr(group, "Particle textures"))
				{
					pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
				}
			}
			done = false;
		}
	}

}

float wallAlpha;
float modelalpha;
float propAlpha;
bool prepared;

void visuals::DoAsusWalls()
{

	C_BaseEntity *pLocal = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());


	if (!pLocal || !g_Engine->IsConnected() || !g_Engine->IsInGame())
		return;

	if (g_Options.Visuals.wallalpha != wallAlpha) {
		for (MaterialHandle_t i = g_MaterialSystem->FirstMaterial(); i != g_MaterialSystem->InvalidMaterial(); i = g_MaterialSystem->NextMaterial(i))
		{
			IMaterial *pMaterial = g_MaterialSystem->GetMaterial(i);

			if (!pMaterial)
				continue;

			if (strstr(pMaterial->GetTextureGroupName(), "World textures"))
				pMaterial->AlphaModulate(g_Options.Visuals.wallalpha);
		}
		wallAlpha = g_Options.Visuals.wallalpha;
	}

	if (g_Options.Visuals.propalpha != propAlpha) {
		for (MaterialHandle_t i = g_MaterialSystem->FirstMaterial(); i != g_MaterialSystem->InvalidMaterial(); i = g_MaterialSystem->NextMaterial(i))
		{
			IMaterial *pMaterial = g_MaterialSystem->GetMaterial(i);

			if (!pMaterial)
				continue;

			if (strstr(pMaterial->GetTextureGroupName(), "StaticProp textures")) {
				pMaterial->AlphaModulate(g_Options.Visuals.propalpha);
			}
		}
		propAlpha = g_Options.Visuals.propalpha;
	}

	if (g_Options.Visuals.modelalpha != modelalpha) {
		for (MaterialHandle_t i = g_MaterialSystem->FirstMaterial(); i != g_MaterialSystem->InvalidMaterial(); i = g_MaterialSystem->NextMaterial(i))
		{
			IMaterial *pMaterial = g_MaterialSystem->GetMaterial(i);

			if (!pMaterial)
				continue;

			if (strstr(pMaterial->GetTextureGroupName(), "Model textures"))
				pMaterial->AlphaModulate(g_Options.Visuals.modelalpha);
		}
		modelalpha = g_Options.Visuals.modelalpha;
	}

}

void visuals::renderBeams()
{

	if (g_Options.Visuals.bulletshow)
		return;

	auto local = static_cast<C_BaseEntity*>(g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer()));
	if (!local)
		return;

	for (size_t i = 0; i < logs.size(); i++)
	{

		auto current = logs.at(i);

		current.color = Color(g_Options.Colors.BulletTracer);

		if (g_Options.Visuals.bulletshow)
		{
			g_DebugOverlay->AddLineOverlay(current.src, current.dst, current.color.r(), current.color.g(), current.color.b(), true, -1);
			g_DebugOverlay->AddBoxOverlay(current.dst, Vector(-2, -2, -2), Vector(2, 2, 2), Vector(0, 0, 0), current.color.r(), current.color.g(), current.color.b(), 127, -1);
		}

		if (fabs(g_Globals->curtime - current.time) > 5.f)
			logs.erase(logs.begin() + i);
	}
}

void visuals::DrawLogs() 
{
	for (size_t i = 0; i < events.size(); i++) 
	{
		float time = g_Globals->curtime - events[i].time;

		if (time > 6.f)
			events[i].color[3] *= 0.98f; //[3]

		Color color = Color(int(g_Options.Colors.MenuTheme[0] * 255.f), int(g_Options.Colors.MenuTheme[1] * 255.f), int(g_Options.Colors.MenuTheme[2] * 255.f), events[i].color[3]);

		g_Render->Textf(5, 5 + i * 17, Color(255, 10, 10, events[i].color[3]), g_Render->font.EventLogs, "[TornadoNight]");
		g_Render->Textf(75, 5 + i * 17, events[i].color, g_Render->font.EventLogs, events[i].text.c_str());

	}

}