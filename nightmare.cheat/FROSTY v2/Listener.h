#pragma once

typedef __int64 int64;

#include "singleton.hpp"
#include "MiscClasses.h"
#include "Interfaces.h"
#include "Interface.h"
#include "Sounds.h"
#include "DamageIndicator.h"

char* HitgroupToName(int hitgroup)
{
	switch (hitgroup)
	{
	case 1:
		return "head";
	case 6:
		return "arm";
	case 7:
		return "leg";
	case 3:
		return "stomach";
	default:
		return "body";
	}
}
const char* hasdefusekit[]{ "without a defuse kit","with a defuse kit" };
std::vector<cbullet_tracer_info> logs;
template <typename t> t xd(t value, t min, t max) {
	if (value > max) {
		return max;
	}
	if (value < min) {
		return min;
	}
	return value;
}
#pragma comment(lib, "winmm.lib")
class item_purchase : public singleton<item_purchase>
{
	class item_purchase_listener : public IGameEventListener2
	{
	public:
		void start()
		{
			g_EventManager->AddListener(this, "item_purchase", false);
			g_EventManager->AddListener(this, "player_hurt", false);
			g_EventManager->AddListener(this, "player_death", false);
			g_EventManager->AddListener(this, "bullet_impact", false);
			g_EventManager->AddListener(this, "bomb_beginplant", false);
			g_EventManager->AddListener(this, "bomb_begindefuse", false);
			g_EventManager->AddListener(this, "bomb_abortplant", false);
			g_EventManager->AddListener(this, "weapon_fire", false);
		}
		void stop()
		{
			g_EventManager->RemoveListener(this);
		}
		void FireGameEvent(IGameEvent *event) override
		{
			singleton()->on_fire_event(event);
		}
		int GetEventDebugID(void) override
		{
			return 42 /*0x2A*/;
		}
	};

public:

	static item_purchase* singleton()
	{
		static item_purchase* instance = new item_purchase;
		return instance;
	}

	void initialize()
	{
		listener.start();
	}

	void remove()
	{
		listener.stop();
	}

	void on_fire_event(IGameEvent* event)
	{

		if (!strcmp(event->GetName(), "item_purchase"))
		{
			C_BaseEntity* local = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
			auto buyer = event->GetInt("userid");
			std::string gun = event->GetString("weapon");

			if (strstr(gun.c_str(), "molotov")
				|| strstr(gun.c_str(), "nade")
				|| strstr(gun.c_str(), "kevlar")
				|| strstr(gun.c_str(), "decoy")
				|| strstr(gun.c_str(), "suit")
				|| strstr(gun.c_str(), "flash")
				|| strstr(gun.c_str(), "vest")
				|| strstr(gun.c_str(), "cutter")
				|| strstr(gun.c_str(), "defuse")
				)  return;

			auto player_index = g_Engine->GetPlayerForUserID(buyer);
			C_BaseEntity* player = (C_BaseEntity*)g_EntityList->GetClientEntity(player_index);
			player_info_t pinfo;

			if (player && local && g_Engine->GetPlayerInfo(player_index, &pinfo))
			{
	
				if (g_Options.Misc.EventLog.Enable && g_Options.Misc.EventLog.ItemPurchase)
				{
					gun.erase(gun.find("weapon_"), 7);
					if (player->GetTeamNum() != local->GetTeamNum())
					{
//						G::Msg("[TornadoNight] %s bought %s\n", pinfo.name, gun.c_str());
						g_CVar->ConsoleColorPrintf(Color(int(g_Options.Colors.MenuTheme[0] * 255.f), int(g_Options.Colors.MenuTheme[1] * 255.f), int(g_Options.Colors.MenuTheme[2] * 255.f)), "[TornadoNight] ");
						g_CVar->ConsoleColorPrintf(Color(255, 255, 255), """%s bought %s\n", pinfo.name, gun.c_str());
						std::stringstream string;
						string << pinfo.name << " bought " << gun.c_str();
						visuals::instance().events.push_front(visuals::loginfo(string.str(), Color(255, 255, 255), g_Globals->curtime));

						if (g_Options.Misc.EventLog.Tick)
							g_Engine->ClientCmd_Unrestricted("play buttons\\button22.wav");
						
					}
				}
			}

		}
		if (!strcmp(event->GetName(), "player_death"))
		{
			if (g_Engine->GetPlayerForUserID(event->GetInt("attacker")) == g_Engine->GetLocalPlayer() &&
				g_Engine->GetPlayerForUserID(event->GetInt("userid")) != g_Engine->GetLocalPlayer())
			{
				if (g_Options.Visuals.Hitbox)
					visuals::Hitbox(g_Engine->GetPlayerForUserID(event->GetInt("userid")));
			}

			if (!event)
				return;

			auto local_player = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
			if (!local_player)
				return;

			auto entity = g_EntityList->GetClientEntity(g_Engine->GetPlayerForUserID(event->GetInt("userid")));

			if (!entity)
				return;

			auto entity_attacker = g_EntityList->GetClientEntity(g_Engine->GetPlayerForUserID(event->GetInt("attacker")));

			if (!entity_attacker)
				return;

			if (entity->GetTeamNum() == local_player->GetTeamNum())
				return;

			player_info_t player_info;
			if (!g_Engine->GetPlayerInfo(entity->GetIndex(), &player_info))
				return;

			if (g_Options.Misc.FakeMedia && entity_attacker == local_player)
			{
				if (!event->GetBool("headshot"))
					event->SetInt("headshot", 1);
			}
		}
		if (!strcmp(event->GetName(), "player_hurt"))
		{
			auto userid = event->GetInt("userid");
			auto attacker = event->GetInt("attacker");
			int dmg = event->GetInt("dmg_health");

			C_BaseEntity* local = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
			auto userid_index = g_Engine->GetPlayerForUserID(userid);
			auto attacker_index = g_Engine->GetPlayerForUserID(attacker);
			C_BaseEntity* userid_ = (C_BaseEntity*)g_EntityList->GetClientEntity(userid_index);
			C_BaseEntity* attacker_ = (C_BaseEntity*)g_EntityList->GetClientEntity(attacker_index);

			if (attacker_ == local && g_Engine->IsConnected() && g_Engine->IsInGame() && local->IsAlive())
			{
				G::hitmarkeralpha = 1.f;
				switch (g_Options.Misc.Hitsound)
				{
				case 0: break;
				case 1: PlaySoundA(COD, NULL, SND_ASYNC | SND_MEMORY); break; // COD
				case 2: g_Engine->ClientCmd_Unrestricted("play buttons\\arena_switch_press_02.wav"); break;// Metal
				case 3: PlaySoundA(bameware, NULL, SND_ASYNC | SND_MEMORY); break;
				case 4: PlaySoundA(bubble, NULL, SND_ASYNC | SND_MEMORY); break;
				case 5: PlaySoundA(thunk, NULL, SND_ASYNC | SND_MEMORY); break;
				case 6: PlaySoundA(click, NULL, SND_ASYNC | SND_MEMORY); break;
				}

			}

			if (g_Options.Visuals.DamageIndicator)
			{
				DamageIndicator_t DmgIndicator;
				DmgIndicator.iDamage = dmg;
				DmgIndicator.Player = userid_;
				DmgIndicator.flEraseTime = local->GetTickBase() * g_Globals->interval_per_tick + 3.f;
				DmgIndicator.bInitialized = false;
				damage_indicators.data.push_back(DmgIndicator);
			}


			if (g_Options.Misc.EventLog.Enable && g_Options.Misc.EventLog.Damage)
			{

				int iAttacker = g_Engine->GetPlayerForUserID(event->GetInt("attacker"));
				int iVictim = g_Engine->GetPlayerForUserID(event->GetInt("userid"));

				if (iAttacker == g_Engine->GetLocalPlayer() && iVictim != g_Engine->GetLocalPlayer())
				{

					auto pVictim = reinterpret_cast<C_BaseEntity*>(g_EntityList->GetClientEntity(iVictim));
					player_info_t pinfo;
					g_Engine->GetPlayerInfo(iVictim, &pinfo);

					g_CVar->ConsoleColorPrintf(Color(int(g_Options.Colors.MenuTheme[0] * 255.f), int(g_Options.Colors.MenuTheme[1] * 255.f), int(g_Options.Colors.MenuTheme[2] * 255.f)), "[TornadoNight] ");
					g_CVar->ConsoleColorPrintf(Color(255, 255, 255), """%s hit in the %s for %d\n", pinfo.name, HitgroupToName(event->GetInt("hitgroup")), event->GetInt("dmg_health"));
					std::stringstream string;
					string << pinfo.name << " hit in the " << HitgroupToName(event->GetInt("hitgroup")) << " for " << event->GetInt("dmg_health");
					visuals::instance().events.push_front(visuals::loginfo(string.str(), Color(255, 255, 255), g_Globals->curtime));
					
				}

			}

		}

		if (!strcmp(event->GetName(), "bullet_impact"))
		{	
			C_BaseEntity* localplayer = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());


			if (localplayer)
			{
				
				auto index = g_Engine->GetPlayerForUserID(event->GetInt("userid"));

				if (index != g_Engine->GetLocalPlayer())
					return;

				auto local = static_cast<C_BaseEntity*>(g_EntityList->GetClientEntity(index));
				if (!local)
					return;

				Vector position(event->GetFloat("x"), event->GetFloat("y"), event->GetFloat("z"));

				Ray_t ray;
				ray.Init(local->GetEyePosition(), position);

				CTraceFilter filter;
				filter.pSkip = local;

				trace_t tr;
				g_EngineTrace->TraceRay(ray, MASK_SHOT, &filter, &tr);

				logs.push_back(cbullet_tracer_info(local->GetEyePosition(), position, g_Globals->curtime, g_Options.Colors.BulletTracer));
					

				if (!local)
					return;

				for (size_t i = 0; i < logs.size(); i++)
				{
					auto current = logs.at(i);
					current.color = Color(g_Options.Colors.BulletTracer); //color of local player's tracers
					BeamInfo_t beamInfo;
					beamInfo.m_nType = TE_BEAMPOINTS;
					beamInfo.m_pszModelName = "sprites/physbeam.vmt";
					beamInfo.m_nModelIndex = -1;
					beamInfo.m_flHaloScale = 0.0f;
					beamInfo.m_flLife = g_Options.Visuals.flTracersDuration;
					beamInfo.m_flWidth = g_Options.Visuals.flTracersWidth;
					beamInfo.m_flEndWidth = g_Options.Visuals.flTracersWidth;
					beamInfo.m_flFadeLength = 0.0f;
					beamInfo.m_flAmplitude = 2.0f;
					beamInfo.m_flBrightness = 255.f;
					beamInfo.m_flSpeed = 0.2f;
					beamInfo.m_nStartFrame = 0;
					beamInfo.m_flFrameRate = 0.f;
					beamInfo.m_flRed = current.color.r();
					beamInfo.m_flGreen = current.color.g();
					beamInfo.m_flBlue = current.color.b();
					beamInfo.m_nSegments = 2;
					beamInfo.m_bRenderable = true;
					beamInfo.m_nFlags = FBEAM_ONLYNOISEONCE | FBEAM_NOTILE | FBEAM_HALOBEAM;

					beamInfo.m_vecStart = localplayer->GetEyePosition();
					beamInfo.m_vecEnd = current.dst;
					if (g_Options.Visuals.bulletshow && g_Options.Visuals.Enabled)
					{
						auto beam = g_pViewRenderBeams->CreateBeamPoints(beamInfo);
						if (beam)
							g_pViewRenderBeams->DrawBeam(beam);

						g_DebugOverlay->AddBoxOverlay(current.dst, Vector(-2, -2, -2), Vector(2, 2, 2), Vector(0, 0, 0), current.color.r(), current.color.g(), current.color.b(), 120, g_Options.Visuals.flTracersDuration);
					}

					logs.erase(logs.begin() + i);
				}
			}
		
		}
		if (!strcmp(event->GetName(), "bomb_beginplant"))
		{
			if (g_Options.Misc.EventLog.Enable && g_Options.Misc.EventLog.BombPlant)
			{
				auto userid = event->GetInt("userid");
				short consolesite = event->GetInt("site");
				auto site = event->GetInt("site");
				std::string sitename;

				//only casual/competitive maps. adding wingman soon
				//im too fucking lazy to use integers instead of strings but who cares it still works.
				if (site == 278 || site == 451 || site == 334 || site == 317 || site == 260 || site == 228 || site == 439 || site == 93 || site == 79 || site == 150 || site == 218) // order: dust2, mirage, inferno, cache, cobblestone, austria, shipped, train, overpass, nuke, canals
					sitename = "A";
				else if (site == 279 || site == 452 || site == 423 || site == 318 || site == 95 || site == 570 || site == 458 || site == 538 || site == 507 || site == 406 || site == 217) // order: dust2, mirage, inferno, cache, cobblestone, austria, shipped, train, overpass, nuke, canals
					sitename = "B";
				else
					sitename = "";

				if (consolesite == 278 || consolesite == 451 || consolesite == 334 || consolesite == 317 || consolesite == 260 || consolesite == 228 || consolesite == 439 || consolesite == 93 || consolesite == 79 || consolesite == 150 || consolesite == 218) // order: dust2, mirage, inferno, cache, cobblestone, austria, shipped, train, overpass, nuke, canals
					sitename = "A";
				else if (consolesite == 279 || consolesite == 452 || consolesite == 423 || consolesite == 318 || consolesite == 95 || consolesite == 570 || consolesite == 458 || consolesite == 538 || consolesite == 507 || consolesite == 406 || consolesite == 217) // order: dust2, mirage, inferno, cache, cobblestone, austria, shipped, train, overpass, nuke, canals
					sitename = "B";
				else
					sitename = "";

				if (!userid)
					return;

				auto userid_id = g_Engine->GetPlayerForUserID(userid);

				if (userid_id == g_Engine->GetLocalPlayer())
					return;

				player_info_t pinfo;
				g_Engine->GetPlayerInfo(userid_id, &pinfo);

				g_CVar->ConsoleColorPrintf(Color(int(g_Options.Colors.MenuTheme[0] * 255.f), int(g_Options.Colors.MenuTheme[1] * 255.f), int(g_Options.Colors.MenuTheme[2] * 255.f)), "[TornadoNight] ");
				g_CVar->ConsoleColorPrintf(Color(255, 255, 255), """%s has began planting the bomb\n", pinfo.name);
				std::stringstream string;
				string << pinfo.name << " has began planting the bomb";
				visuals::instance().events.push_front(visuals::loginfo(string.str(), Color(255, 255, 255), g_Globals->curtime));

				if (g_Options.Misc.EventLog.Tick)
					g_Engine->ClientCmd_Unrestricted("play buttons\\button22.wav");
			}
		}
		if (!strcmp(event->GetName(), "bomb_begindefuse"))
		{
			if (g_Options.Misc.EventLog.Enable && g_Options.Misc.EventLog.BombDefuse)
			{
				auto userid = event->GetInt("userid");

				if (!userid)
					return;

				auto userid_id = g_Engine->GetPlayerForUserID(userid);

				if (userid_id == g_Engine->GetLocalPlayer())
					return;

				player_info_t pinfo;
				g_Engine->GetPlayerInfo(userid_id, &pinfo);

				g_CVar->ConsoleColorPrintf(Color(int(g_Options.Colors.MenuTheme[0] * 255.f), int(g_Options.Colors.MenuTheme[1] * 255.f), int(g_Options.Colors.MenuTheme[2] * 255.f)), "[TornadoNight] ");
				g_CVar->ConsoleColorPrintf(Color(255, 255, 255), """%s has began defusing the bomb %s \n", pinfo.name, hasdefusekit[event->GetBool("haskit")]);
				std::stringstream string;
				string << pinfo.name << " has began defusing the bomb " << hasdefusekit[event->GetBool("haskit")];
				visuals::instance().events.push_front(visuals::loginfo(string.str(), Color(255, 255, 255), g_Globals->curtime));

				if (g_Options.Misc.EventLog.Tick)
					g_Engine->ClientCmd_Unrestricted("play buttons\\button22.wav");
			}
		}
		if (!strcmp(event->GetName(), "bomb_abortplant"))
		{
			if (g_Options.Misc.EventLog.Enable && g_Options.Misc.EventLog.AbortPlant)
			{
				auto userid = event->GetInt("userid");

				if (!userid)
					return;

				auto userid_id = g_Engine->GetPlayerForUserID(userid);
				C_BaseEntity* player = (C_BaseEntity*)g_EntityList->GetClientEntity(userid_id);
				C_BaseEntity* local = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());

				if (userid_id == g_Engine->GetLocalPlayer() || player->GetTeamNum() == local->GetTeamNum())
					return;

				player_info_t pinfo;
				g_Engine->GetPlayerInfo(userid_id, &pinfo);

				g_CVar->ConsoleColorPrintf(Color(int(g_Options.Colors.MenuTheme[0] * 255.f), int(g_Options.Colors.MenuTheme[1] * 255.f), int(g_Options.Colors.MenuTheme[2] * 255.f)), "[TornadoNight] ");
				g_CVar->ConsoleColorPrintf(Color(255, 255, 255), """%s has aborted the bomb plant\n", pinfo.name);
				std::stringstream string;
				string << pinfo.name << " has aborted the bomb plant";
				visuals::instance().events.push_front(visuals::loginfo(string.str(), Color(255, 255, 255), g_Globals->curtime));

				if (g_Options.Misc.EventLog.Tick)
					g_Engine->ClientCmd_Unrestricted("play buttons\\button22.wav");
			}
		}

		if (!strcmp(event->GetName(), "weapon_fire"))
		{
			auto index = g_Engine->GetPlayerForUserID(event->GetInt("userid"));

			if (index != g_Engine->GetLocalPlayer())
				return;

			C_BaseEntity* local = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
			CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)g_EntityList->GetClientEntityFromHandle(local->GetActiveWeaponHandle());

			auto accuracy = pWeapon->GetInaccuracy() * 550.f;

			if (!(!strcmp(event->GetName(), "player_hurt")))
			{
				Globals::missedshots++;

				if (accuracy > Globals::Hitchance && g_Options.Ragebot.Hitchance && g_Options.Ragebot.Enabled)
				{
					g_CVar->ConsoleColorPrintf(Color(int(g_Options.Colors.MenuTheme[0] * 255.f), int(g_Options.Colors.MenuTheme[1] * 255.f), int(g_Options.Colors.MenuTheme[2] * 255.f)), "[TornadoNight] ");
					g_CVar->ConsoleColorPrintf(Color(255, 255, 255), """missed shot due to spread\n");
					std::stringstream string;
					string << "missed shot due to spread";
					visuals::instance().events.push_front(visuals::loginfo(string.str(), Color(255, 255, 255), g_Globals->curtime));
				}
			}
			else if (!strcmp(event->GetName(), "player_hurt"))
				Globals::missedshots = 0;
			
		}

		if (visuals::instance().events.size() > 12)
			visuals::instance().events.pop_back();
	
	}

	

private:
	item_purchase_listener  listener;
};

item_purchase purchase;