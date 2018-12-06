#include "Glow.h"
#include "HookIncludes.h"

void Glow::RenderGlow()
{
	for (auto i = 0; i < g_GlowObjManager->m_GlowObjectDefinitions.Count(); i++)
	{
		auto &glowObject = g_GlowObjManager->m_GlowObjectDefinitions[i];
		auto entity = reinterpret_cast<C_BaseEntity*>(glowObject.m_pEntity);

		if (glowObject.IsUnused())
			continue;

		if (!entity)
			continue;

		if (!g_Options.Visuals.Glow.Enable)
			continue;

		auto class_id = entity->GetClientClass()->m_ClassID;
		Color color;
		C_BaseEntity* g_LocalPlayer = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
		switch (class_id)
		{
		case 35://player
		{
			bool is_enemy = entity->GetTeamNum() != g_LocalPlayer->GetTeamNum();

			if (!g_Options.Visuals.Glow.Players || !entity->IsAlive()) // || !visuals::ValidPlayer(entity
				continue;

			if (!is_enemy && !g_Options.Visuals.TeamESP)
				continue;

			glowObject.m_nGlowStyle = g_Options.Visuals.Glow.Style.Players;

			float* cur_color = (is_enemy ? g_Options.Colors.GlowEnemy : g_Options.Colors.GlowTeam);

			color = Color(cur_color);

			break;
		}
		case 2://cbaseanimating
		{
			if (!g_Options.Visuals.Glow.Others)
				continue;

			glowObject.m_nGlowStyle = g_Options.Visuals.Glow.Style.Others;

			color = Color(0, 0, 255, 255);

			break;
		}
		case 29://bomb
		case 108://bomb
		{
			if (!g_Options.Visuals.Glow.Others)
				continue;

			glowObject.m_nGlowStyle = g_Options.Visuals.Glow.Style.Others;

			color = Color(255, 0, 0, 255);

			break;
		}
		case 1:
		case 39:
		{
			if (entity->IsWeapon())
			{
				if (!g_Options.Visuals.Glow.Others)
					continue;

				glowObject.m_nGlowStyle = g_Options.Visuals.Glow.Style.Others;

				color = Color(70, 255, 70, 255);
			}
			break;
		}
		default://weapons
		{
			if (entity->IsWeapon())
			{
				if (!g_Options.Visuals.Glow.Others)
					continue;

				glowObject.m_nGlowStyle = g_Options.Visuals.Glow.Style.Others;

				color = Color(70, 255, 70, 255);
			}
		}
		}

		glowObject.m_flRed = color.r() / 255.0f;
		glowObject.m_flGreen = color.g() / 255.0f;
		glowObject.m_flBlue = color.b() / 255.0f;
		glowObject.m_flAlpha = color.a() / 255.0f;
		glowObject.m_bRenderWhenOccluded = true;
		glowObject.m_bRenderWhenUnoccluded = false;
	}
}

void Glow::ClearGlow()
{
	for (auto i = 0; i < g_GlowObjManager->m_GlowObjectDefinitions.Count(); i++)
	{
		auto &glowObject = g_GlowObjManager->m_GlowObjectDefinitions[i];
		auto entity = reinterpret_cast<C_BaseEntity*>(glowObject.m_pEntity);

		if (glowObject.IsUnused())
			continue;

		if (!entity || entity->IsDormant())
			continue;

		glowObject.m_flAlpha = 0.0f;
	}
}