#include "Configuration.hpp"
#include "Variables.h"
#include <winerror.h>
#pragma warning( disable : 4091)
#include <ShlObj.h>
#include <string>
#include <sstream>
#include "XorStr.hpp"


const char* GetWeaponName(int iWeaponID)
{
	switch (iWeaponID)
	{
	default:
		return "none";
	case WEAPON_DEAGLE:
		return "deagle";
	case WEAPON_DUALS:
		return "duals";
	case WEAPON_FIVE7:
		return "five7";
	case WEAPON_GLOCK:
		return "glock";
	case WEAPON_AK47:
		return "ak47";
	case WEAPON_AUG:
		return "aug";
	case WEAPON_AWP:
		return "awp";
	case WEAPON_FAMAS:
		return "famas";
	case WEAPON_G3SG1:
		return "g3sg1";
	case WEAPON_GALIL:
		return "galil";
	case WEAPON_M249:
		return "m249";
	case WEAPON_M4A1:
		return "m4a4";
	case WEAPON_MAC10:
		return "mac10";
	case WEAPON_P90:
		return "p90";
	case WEAPON_UMP45:
		return "ump45";
	case WEAPON_XM1014:
		return "xm1014";
	case WEAPON_BIZON:
		return "bizon";
	case WEAPON_MAG7:
		return "mag7";
	case WEAPON_NEGEV:
		return "negev";
	case WEAPON_SAWEDOFF:
		return "sawedoff";
	case WEAPON_TEC9:
		return "tec9";
	case WEAPON_TASER:
		return "taser";
	case WEAPON_USPS:
		return "usp-s";
	case WEAPON_MP7:
		return "mp7";
	case WEAPON_MP9:
		return "mp9";
	case WEAPON_NOVA:
		return "nova";
	case WEAPON_P250:
		return "p250";
	case WEAPON_SCAR20:
		return "scar20";
	case WEAPON_SG553:
		return "sg556";
	case WEAPON_SCOUT:
		return "ssg08";
	case WEAPON_M4A1S:
		return "m4a1-s";
	case WEAPON_P2000:
		return "p2000";
	case WEAPON_CZ75:
		return "cz75";
	case WEAPON_REVOLVER:
		return "revolver";
	case WEAPON_KNIFE_BAYONET:
		return "bayonet";
	case WEAPON_KNIFE_BUTTERFLY:
		return "butterfly";
	case WEAPON_KNIFE_FALCHION:
		return "falshion";
	case WEAPON_KNIFE_FLIP:
		return "flip";
	case WEAPON_KNIFE_GUT:
		return "gut";
	case WEAPON_KNIFE_KARAMBIT:
		return "karambit";
	case WEAPON_KNIFE_M9BAYONET:
		return "m9";
	case WEAPON_KNIFE_HUNTSMAN:
		return "huntsman";
	case WEAPON_KNIFE_BOWIE:
		return "bowie";
	case WEAPON_KNIFE_DAGGER:
		return "daggers";
	}
}


void CConfig::Setup()
{
	//RAGEBOT
	SetupValue(g_Options.Ragebot.MainSwitch, false, ("Ragebot"), ("RageToggle"));
	SetupValue(g_Options.Ragebot.Enabled, false, ("Ragebot"), ("Enabled"));
	SetupValue(g_Options.Ragebot.AutoFire, false, ("Ragebot"), ("AutoFire"));
	SetupValue(g_Options.Ragebot.AutoRevolver, false, ("Ragebot"), ("AutoRevolver"));
	SetupValue(g_Options.Ragebot.EnginePrediction, false, ("Ragebot"), ("EnginePrediction"));
	SetupValue(g_Options.Ragebot.FOV, 0.f, ("Ragebot"), ("FOV"));
	SetupValue(g_Options.Ragebot.Silent, false, ("Ragebot"), ("Silent"));
	SetupValue(g_Options.Ragebot.AutoPistol, false, ("Ragebot"), ("AutoPistol"));
	SetupValue(g_Options.Ragebot.KeyPress, 0, ("Ragebot"), ("AutoshootKey"));
	SetupValue(g_Options.Ragebot.fliponkey, false, ("Ragebot"), ("FlipEnable"));
	SetupValue(g_Options.Ragebot.flipkey, 0, ("Ragebot"), ("FlipKey"));

	SetupValue(g_Options.Ragebot.EnabledAntiAim, false, ("Ragebot"), ("AntiaimEnabled"));
	SetupValue(g_Options.Ragebot.Pitch, 0, ("Ragebot"), ("AntiaimPitch"));
	SetupValue(g_Options.Ragebot.YawTrue, 0, ("Ragebot"), ("AntiaimYaw-true"));
	SetupValue(g_Options.Ragebot.YawFake, 0, ("Ragebot"), ("AntiaimYaw-fake"));
	SetupValue(g_Options.Ragebot.AtTarget, false, ("Ragebot"), ("attargets"));
	SetupValue(g_Options.Ragebot.Edge, false, ("Ragebot"), ("edge"));
	SetupValue(g_Options.Ragebot.KnifeAA, false, ("Ragebot"), ("KnifeAA"));
	SetupValue(g_Options.Ragebot.LBYBreak, false, ("Ragebot"), ("LBY-Breaker"));
	SetupValue(g_Options.Ragebot.LBYBreakDelta, 0.f, ("Ragebot"), ("LBY-Breaker Delta"));
	SetupValue(g_Options.Ragebot.FakeLag, false, ("Ragebot"), ("Fakelag"));
	SetupValue(g_Options.Ragebot.FakeLagAmt, 0, ("Ragebot"), ("Fakelag Amount"));
	SetupValue(g_Options.Ragebot.walktype, false, ("Ragebot"), ("WalkType"));
	SetupValue(g_Options.Ragebot.fakewalkkey, 0, ("Ragebot"), ("Fakewalk Key"));

	SetupValue(g_Options.Ragebot.FriendlyFire, false, ("Ragebot"), ("FriendlyFire"));
	SetupValue(g_Options.Ragebot.Priority, 0, ("Ragebot"), ("Priority"));
	SetupValue(g_Options.Ragebot.Hitscan, 0, ("Ragebot"), ("Hitscan"));

	SetupValue(g_Options.Ragebot.AntiRecoil, false, ("Ragebot"), ("AntiRecoil"));
	SetupValue(g_Options.Ragebot.AutoStop, 0, ("Ragebot"), ("AutoStop"));
	SetupValue(g_Options.Ragebot.AutoCrouch, false, ("Ragebot"), ("AutoCrouch"));
	SetupValue(g_Options.Ragebot.AutoScope, false, ("Ragebot"), ("AutoScope"));
	SetupValue(g_Options.Ragebot.MinimumDamageSniper, 0.f, ("Ragebot"), ("AutoWallDamageSniper"));
	SetupValue(g_Options.Ragebot.MinimumDamagePistol, 0.f, ("Ragebot"), ("AutoWallDamagePistol"));
	SetupValue(g_Options.Ragebot.MinimumDamageRifle, 0.f, ("Ragebot"), ("AutoWallDamageRifle"));
	SetupValue(g_Options.Ragebot.MinimumDamageHeavy, 0.f, ("Ragebot"), ("AutoWallDamageHeavy"));
	SetupValue(g_Options.Ragebot.MinimumDamageSmg, 0.f, ("Ragebot"), ("AutoWallDamageSmgs"));
	SetupValue(g_Options.Ragebot.MinimumDamageRevolver, 0.f, ("Ragebot"), ("AutoWallDamageRevolver"));
	SetupValue(g_Options.Ragebot.Hitchance, false, ("Ragebot"), ("HitChance"));
	SetupValue(g_Options.Ragebot.HitchanceSniper, 0.f, ("Ragebot"), ("HitChanceSniper"));
	SetupValue(g_Options.Ragebot.HitchancePistol, 0.f, ("Ragebot"), ("HitChancePistol"));
	SetupValue(g_Options.Ragebot.HitchanceHeavy, 0.f, ("Ragebot"), ("HitChanceHeavy"));
	SetupValue(g_Options.Ragebot.HitchanceSmgs, 0.f, ("Ragebot"), ("HitChanceSmgs"));
	SetupValue(g_Options.Ragebot.HitchanceRifle, 0.f, ("Ragebot"), ("HitChanceRifle"));
	SetupValue(g_Options.Ragebot.HitchanceRevolver, 0.f, ("Ragebot"), ("HitChanceRevolver"));
	SetupValue(g_Options.Ragebot.Resolver, false, ("Ragebot"), ("Resolver"));
	SetupValue(g_Options.Ragebot.bruteAfterX, 0, ("Ragebot"), ("Bruteforce"));
	SetupValue(g_Options.Ragebot.BaimIfMissed, 0, ("Ragebot"), ("BAIMIfMiss"));
	SetupValue(g_Options.Ragebot.Override, false, ("Ragebot"), ("OverrideEnable"));
	SetupValue(g_Options.Ragebot.OverrideKey, 0, ("Ragebot"), ("OverrideKey"));
	SetupValue(g_Options.Ragebot.PitchAdj, 0, ("Ragebot"), ("PitchAdjustment"));
	SetupValue(g_Options.Ragebot.FakeLagFix, false, ("Ragebot"), ("Fakelag Fix"));
	SetupValue(g_Options.Ragebot.PosAdjust, false, ("Ragebot"), ("Position Adjustment"));
	SetupValue(g_Options.Ragebot.BAIMkey, 0, ("Ragebot"), ("BAIMKey"));
	SetupValue(g_Options.Ragebot.BAIMIfLethal, false, ("Ragebot"), ("BAIMIfLethal"));



	//LEGITBOT
	SetupValue(g_Options.LegitBot.Enable, false, ("Legitbot"), ("Enabled"));
	SetupValue(g_Options.LegitBot.AutoPistol, false, ("Legitbot"), ("AutoPistol"));
	SetupValue(g_Options.LegitBot.RCSFix, false, ("Legitbot"), ("RCSFix"));

	//BACKTRACK
	SetupValue(g_Options.Backtrack.backtrackenable, false, ("Legitbot"), ("Backtrack"));
	SetupValue(g_Options.Backtrack.BacktrackType, 0, ("Legitbot"), ("BacktrackType"));
	SetupValue(g_Options.Backtrack.backtrackticks, 0, ("Legitbot"), ("BacktrackTicks"));


	SetupValue(g_Options.LegitBot.MainKey, 1, ("Legitbot"), ("Key-Rifles"));
	SetupValue(g_Options.LegitBot.Mainfov, 0.f, ("Legitbot"), ("FOV-Rifles"));
	SetupValue(g_Options.LegitBot.MainSmooth, 1.f, ("Legitbot"), ("Smooth-Rifles"));
	SetupValue(g_Options.LegitBot.main_recoil_min, 0, ("Legitbot"), ("RCS-min-Rifles"));
	SetupValue(g_Options.LegitBot.main_recoil_max, 0, ("Legitbot"), ("RCS-max-Rifles"));

	SetupValue(g_Options.LegitBot.PistolKey, 6, ("Legitbot"), ("Key-Pistol"));
	SetupValue(g_Options.LegitBot.Pistolfov, 0.f, ("Legitbot"), ("FOV-Pistol"));
	SetupValue(g_Options.LegitBot.PistolSmooth, 1.f, ("Legitbot"), ("Smooth-Pistol"));
	SetupValue(g_Options.LegitBot.pistol_recoil_min, 0, ("Legitbot"), ("RCS-min-pistol"));
	SetupValue(g_Options.LegitBot.pistol_recoil_max, 0, ("Legitbot"), ("RCS-max-pistol"));

	SetupValue(g_Options.LegitBot.SniperKey, 6, ("Legitbot"), ("Key-Sniper"));
	SetupValue(g_Options.LegitBot.Sniperfov, 0.f, ("Legitbot"), ("FOV-Sniper"));
	SetupValue(g_Options.LegitBot.SniperSmooth, 0.f, ("Legitbot"), ("Smooth-Sniper"));
	SetupValue(g_Options.LegitBot.sniper_recoil_min, 0, ("Legitbot"), ("RCS-min-sniper"));
	SetupValue(g_Options.LegitBot.sniper_recoil_max, 0, ("Legitbot"), ("RCS-max-sniper"));

	//WEAPONS SETTS 
	SetupValue(g_Options.LegitBot.FilterMainWeapons.Headwpmain, false, ("Legitbot"), ("Headwpmain"));
	SetupValue(g_Options.LegitBot.FilterMainWeapons.Neckwpmain, false, ("Legitbot"), ("Neckwpmain"));
	SetupValue(g_Options.LegitBot.FilterMainWeapons.Chestwpmain, false, ("Legitbot"), ("Chestwpmain"));
	SetupValue(g_Options.LegitBot.FilterMainWeapons.Stomachwpmain, false, ("Legitbot"), ("Stomachwpmain"));
	SetupValue(g_Options.LegitBot.FilterMainWeapons.multiboneswpmain, false, ("Legitbot"), ("multiboneswpmain"));

	//PISTOLS
	SetupValue(g_Options.LegitBot.FilterPistolWeapons.Headwppis, false, ("Legitbot"), ("Headwppis"));
	SetupValue(g_Options.LegitBot.FilterPistolWeapons.Neckwppis, false, ("Legitbot"), ("Neckwppis"));
	SetupValue(g_Options.LegitBot.FilterPistolWeapons.Chestwppis, false, ("Legitbot"), ("Chestwppis"));
	SetupValue(g_Options.LegitBot.FilterPistolWeapons.Stomachwppis, false, ("Legitbot"), ("Stomachwppis"));
	SetupValue(g_Options.LegitBot.FilterPistolWeapons.multiboneswppis, false, ("Legitbot"), ("multiboneswppis"));

	//SNIPER
	SetupValue(g_Options.LegitBot.FilterSniperWeapons.HeadwpSnip, false, ("Legitbot"), ("HeadwpSnip"));
	SetupValue(g_Options.LegitBot.FilterSniperWeapons.NeckwpSnip, false, ("Legitbot"), ("NeckwpSnip"));
	SetupValue(g_Options.LegitBot.FilterSniperWeapons.ChestwpSnip, false, ("Legitbot"), ("ChestwpSnip"));
	SetupValue(g_Options.LegitBot.FilterSniperWeapons.StomachwpSnip, false, ("Legitbot"), ("StomachwpSnip"));
	SetupValue(g_Options.LegitBot.FilterSniperWeapons.multiboneswpSnip, false, ("Legitbot"), ("multiboneswpSnip"));

	//SMGWEAPONS
	SetupValue(g_Options.LegitBot.FiltersmgWeapons.Headwpsmg, false, ("Legitbot"), ("Headwpsmg"));
	SetupValue(g_Options.LegitBot.FiltersmgWeapons.Neckwpsmg, false, ("Legitbot"), ("Neckwpsmg"));
	SetupValue(g_Options.LegitBot.FiltersmgWeapons.Chestwpsmg, false, ("Legitbot"), ("Chestwpsmg"));
	SetupValue(g_Options.LegitBot.FiltersmgWeapons.Stomachwpsmg, false, ("Legitbot"), ("Stomachwpsmg"));
	SetupValue(g_Options.LegitBot.FiltersmgWeapons.multiboneswpsmg, false, ("Legitbot"), ("multiboneswpsmg"));

	//HEABYWEAPONS
	SetupValue(g_Options.LegitBot.FilterheavyWeapons.Headwphv, false, ("Legitbot"), ("Headwphv"));
	SetupValue(g_Options.LegitBot.FilterheavyWeapons.Neckwphv, false, ("Legitbot"), ("Neckwphv"));
	SetupValue(g_Options.LegitBot.FilterheavyWeapons.Chestwphv, false, ("Legitbot"), ("Chestwphv"));
	SetupValue(g_Options.LegitBot.FilterheavyWeapons.Stomachwphv, false, ("Legitbot"), ("Stomachwphv"));
	SetupValue(g_Options.LegitBot.FilterheavyWeapons.multibonewphv, false, ("Legitbot"), ("multibonewphv"));



	//TRIGGERBOT
	SetupValue(g_Options.LegitBot.Triggerbot.Enabled, false, ("Triggerbot"), ("Enabled"));
	SetupValue(g_Options.LegitBot.Triggerbot.Key, 6, ("Triggerbot"), ("Key"));
	SetupValue(g_Options.LegitBot.Triggerbot.Delay, 0.f, ("Triggerbot"), ("Delay"));
	SetupValue(g_Options.LegitBot.Triggerbot.hitchance, 0.f, ("Triggerbot"), ("Hitchance"));

	SetupValue(g_Options.LegitBot.Triggerbot.Filter.Head, false, ("Triggerbot"), ("FilterHead"));
	SetupValue(g_Options.LegitBot.Triggerbot.Filter.Chest, false, ("Triggerbot"), ("FilterChest"));
	SetupValue(g_Options.LegitBot.Triggerbot.Filter.Stomach, false, ("Triggerbot"), ("FilterStomach"));
	SetupValue(g_Options.LegitBot.Triggerbot.Filter.Arms, false, ("Triggerbot"), ("FilterArms"));
	SetupValue(g_Options.LegitBot.Triggerbot.Filter.Legs, false, ("Triggerbot"), ("FilterLegs"));

	//VISUALS
	SetupValue(g_Options.Visuals.Enabled, false, ("Visuals"), ("Enabled"));
	SetupValue(g_Options.Visuals.TeamESP, false, ("Visuals"), ("TeamESP"));
	SetupValue(g_Options.Visuals.Box, false, ("Visuals"), ("BoxEnabled"));
	SetupValue(g_Options.Visuals.BoxType, 0, ("Visuals"), ("BoxType"));
	SetupValue(g_Options.Visuals.fill, false, ("Visuals"), ("Box-Fill"));
	SetupValue(g_Options.Visuals.esp_fill_amount, 0.f, ("Visuals"), ("Fill-Amount"));
	SetupValue(g_Options.Visuals.health, false, ("Visuals"), ("Health"));
	SetupValue(g_Options.Visuals.healthtype, 0, ("Visuals"), ("HealthType"));
	SetupValue(g_Options.Visuals.armor, false, ("Visuals"), ("Armor"));
	SetupValue(g_Options.Visuals.AmmoBox, false, ("Visuals"), ("Amoo-Bar"));
	SetupValue(g_Options.Visuals.armorflag, false, ("Visuals"), ("ArmorFlag"));
	SetupValue(g_Options.Visuals.Money, false, ("Visuals"), ("Money"));
	SetupValue(g_Options.Visuals.Name, false, ("Visuals"), ("Name"));
	SetupValue(g_Options.Visuals.skeletonenbl, false, ("Visuals"), ("SkeletonEnabled"));
	SetupValue(g_Options.Visuals.skeletonopts, 0, ("Visuals"), ("SkeletonType"));
	SetupValue(g_Options.Visuals.Weapon, 0, ("Visuals"), ("Weapon"));
	SetupValue(g_Options.Visuals.AimLine, false, ("Visuals"), ("AimLine"));
	SetupValue(g_Options.Visuals.angleArrows, false, ("Visuals"), ("angleArrows"));
	SetupValue(g_Options.Visuals.angleLines, false, ("Visuals"), ("AngleLines"));
	SetupValue(g_Options.Visuals.angleLinesName, false, ("Visuals"), ("AngleLinesName"));
	SetupValue(g_Options.Visuals.barrel, false, ("Visuals"), ("Barrel"));
	SetupValue(g_Options.Visuals.barrelL, 0, ("Visuals"), ("Barrel-Amount"));
	SetupValue(g_Options.Visuals.TaserRange, false, ("Visuals"), ("Taser-Range"));
	SetupValue(g_Options.Visuals.bulletshow, false, ("Visuals"), ("BulletImpacts"));
	SetupValue(g_Options.Visuals.flTracersDuration, 0.f, ("Visuals"), ("BulletImpact-Duration"));
	SetupValue(g_Options.Visuals.flTracersWidth, 0.f, ("Visuals"), ("BulletImpact-Width"));
	SetupValue(g_Options.Visuals.Hitbox, false, ("Visuals"), ("Hitbox"));
	SetupValue(g_Options.Visuals.HitboxDuration, 0.f, ("Visuals"), ("HitboxDuration"));
	SetupValue(g_Options.Visuals.HeadPosition, 0, ("Visuals"), ("HeadPos"));
	SetupValue(g_Options.Visuals.HeadPositionSize, 0.f, ("Visuals"), ("HeadPosSize"));
	SetupValue(g_Options.Visuals.viewmodelChanger, 0.f, ("Visuals"), ("Viewmodel-Amount"));
	SetupValue(g_Options.Visuals.C4World, false, ("Visuals"), ("Bomb"));
	SetupValue(g_Options.Visuals.DroppedGuns, false, ("Visuals"), ("DroppedGuns"));
	SetupValue(g_Options.Visuals.DroppedGunsType, 0, ("Visuals"), ("DroppedGunsType"));
	SetupValue(g_Options.Visuals.noscopeborder, false, ("Visuals"), ("Noscopeborder"));
	SetupValue(g_Options.Visuals.BombCarrier, false, ("Visuals"), ("BombCarrier"));
	SetupValue(g_Options.Visuals.Hostage, false, ("Visuals"), ("Hostage"));
	SetupValue(g_Options.Visuals.HostageBox, false, ("Visuals"), ("HostageBox"));
	SetupValue(g_Options.Visuals.Chicken, false, ("Visuals"), ("Chicken"));
	SetupValue(g_Options.Visuals.ChickenBox, false, ("Visuals"), ("ChickenBox"));
	SetupValue(g_Options.Visuals.Flashed, false, ("Visuals"), ("Flashed"));
	SetupValue(g_Options.Visuals.Distance, false, ("Visuals"), ("Distance"));
	SetupValue(g_Options.Visuals.Scoped, false, ("Visuals"), ("Scoped"));
	//	SetupValue(g_Options.Visuals.Reloading, false, ("Visuals"), ("Reloading"));
	SetupValue(g_Options.Visuals.IsHasDefuser, false, ("Visuals"), ("HasDefuser"));
	SetupValue(g_Options.Visuals.IsDefusing, false, ("Visuals"), ("IsDefusing"));
	SetupValue(g_Options.Visuals.Grenades, 0, ("Visuals"), ("Grenades"));
	SetupValue(g_Options.Visuals.GrenadePrediction, false, ("Visuals"), ("GrenadePrediction"));
	SetupValue(g_Options.Visuals.GrenadeBox, false, ("Visuals"), ("GrenadeBox"));
	SetupValue(g_Options.Visuals.Chams, false, ("Visuals"), ("Chams-Enabled"));
	SetupValue(g_Options.Visuals.FakeAngleChams, false, ("Visuals"), ("FakeAngleChams"));

	SetupValue(g_Options.Visuals.Teamchams, false, ("Visuals"), ("Teamchams"));
	SetupValue(g_Options.Visuals.champlayeralpha, 0, ("Visuals"), ("Chams-Alpha"));
	SetupValue(g_Options.Visuals.XQZ, false, ("Visuals"), ("XQZ"));
	SetupValue(g_Options.Visuals.Hands, 0, ("Visuals"), ("Hands"));
	SetupValue(g_Options.Visuals.HandsAlpha, 0, ("Visuals"), ("HandsAlpha"));
	SetupValue(g_Options.Visuals.chamswphands, 0, ("Visuals"), ("HeldWeaponCham"));
	SetupValue(g_Options.Visuals.chamswphandsalpha, 0, ("Visuals"), ("HeldWeaponChamAlpha"));
	SetupValue(g_Options.Visuals.SniperCrosshair, false, ("Visuals"), ("SniperCrosshair"));
	SetupValue(g_Options.Visuals.SniperCrosshairType, 0, ("Visuals"), ("SniperCrosshairType"));
	SetupValue(g_Options.Visuals.RecoilCrosshair, false, ("Visuals"), ("RecoilCrosshair"));
	SetupValue(g_Options.Visuals.RecoilCrosshair2, 0, ("Visuals"), ("RecoilCrosshair2"));
	SetupValue(g_Options.Visuals.AWallCrosshair, false, ("Visuals"), ("A-WallIndicator"));
	SetupValue(g_Options.Visuals.SpreadCrosshair, false, ("Visuals"), ("SpreadCrosshair"));
	SetupValue(g_Options.Visuals.spread_crosshair_amount, 110.f, ("Visuals"), ("Spread-Amount"));
	SetupValue(g_Options.Visuals.NoVisualRecoil, false, ("Visuals"), ("NoVisualRecoil"));
	SetupValue(g_Options.Visuals.FlashAlpha, 0, ("Visuals"), ("FlashAlpha"));
	SetupValue(g_Options.Visuals.Smoke, 0, ("Visuals"), ("Smoke"));
	SetupValue(g_Options.Visuals.viewmodelChanger_enabled, false, ("Visuals"), ("ViewmodelEnabled"));
	SetupValue(g_Options.Visuals.FOVChanger_enabled, false, ("Visuals"), ("FovEnabled"));
	SetupValue(g_Options.Visuals.FOVChanger, 0.f, ("Visuals"), ("FOV-Amount"));
	SetupValue(g_Options.Visuals.viewmodelChanger, 0.f, ("Visuals"), ("Viewmodel-Amount"));
	SetupValue(g_Options.Visuals.OffscreenIndicator, false, ("Visuals"), ("Offscreen-Enable"));
	SetupValue(g_Options.Visuals.OffscreenIndicatorCheck, 0, ("Visuals"), ("Offscreen-Check"));
	SetupValue(g_Options.Visuals.OffscreenIndicatorRadius, 0.f, ("Visuals"), ("Offscreen-Radius"));
	SetupValue(g_Options.Visuals.OffscreenIndicatorSize, 0, ("Visuals"), ("Offscreen-Size"));

	SetupValue(g_Options.Visuals.nightMode, false, ("Visuals"), ("NightMode"));
	SetupValue(g_Options.Visuals.NoPostProcess, false, ("Visuals"), ("Postprocess"));
	SetupValue(g_Options.Visuals.wallalpha, 1.f, ("Visuals"), ("wallalpha"));
	SetupValue(g_Options.Visuals.propalpha, 1.f, ("Visuals"), ("propalpha"));
	SetupValue(g_Options.Visuals.modelalpha, 1.f, ("Visuals"), ("modelalpha"));
	SetupValue(g_Options.Visuals.TPKey, 0, ("Visuals"), ("TP-KEY"));
	//	SetupValue(g_Options.Visuals.ThirdpersonMode, 0, ("Visuals"), ("TP-Mode"));
	SetupValue(g_Options.Visuals.Enabletp, false, ("Visuals"), ("TP-Enable"));
	SetupValue(g_Options.Visuals.transparency, false, ("Visuals"), ("TP-Transparency"));

	SetupValue(g_Options.Visuals.Glow.Enable, false, ("Visuals"), ("Glow-Enable"));
	SetupValue(g_Options.Visuals.Glow.Players, false, ("Visuals"), ("Glow-Players"));
	SetupValue(g_Options.Visuals.Glow.Others, false, ("Visuals"), ("Glow-Others"));

	SetupValue(g_Options.Visuals.Glow.Style.Players, 0, ("Visuals"), ("Glow-Style-Players"));
	SetupValue(g_Options.Visuals.Glow.Style.Others, 0, ("Visuals"), ("Glow-Style-Other"));

	SetupValue(g_Options.Visuals.DamageIndicator, false, ("Visuals"), ("DamageIndicator"));

	SetupValue(g_Options.Visuals.LBYIndicator, false, ("Visuals"), ("LBYIndicator"));

	SetupValue(g_Options.Misc.syncclantag, false, ("Misc"), ("Sync-Clantag"));
	SetupValue(g_Options.Misc.namePrometheus, false, ("Misc"), ("namePrometheus"));
	SetupValue(g_Options.Misc.Bhop, false, ("Misc"), ("Bhop"));
	SetupValue(g_Options.Misc.AutoStrafe, 0, ("Misc"), ("Autostrafe"));
	SetupValue(g_Options.Misc.spammer, 0, ("Visuals"), ("Spammer"));
	SetupValue(g_Options.Misc.spammeron, false, ("Misc"), ("SpammerEnabled"));
	SetupValue(g_Options.Misc.SpecList, false, ("Misc"), ("SpecList"));
	SetupValue(g_Options.Misc.ServerRankRevealAll, false, ("Misc"), ("RankRevealAll"));
	SetupValue(g_Options.Misc.AutoAccept, false, ("Misc"), ("AutoAccept"));
	SetupValue(g_Options.Misc.Hitmarker, 0, ("Misc"), ("Hitmarker"));
	SetupValue(g_Options.Misc.Hitsound, 0, ("Misc"), ("HitSound"));

	SetupValue(g_Options.Misc.EventLog.Enable, false, ("Misc"), ("LogEnable"));
	SetupValue(g_Options.Misc.EventLog.ItemPurchase, false, ("Misc"), ("LogPurchase"));
	SetupValue(g_Options.Misc.EventLog.Damage, false, ("Misc"), ("LogDamage"));
	SetupValue(g_Options.Misc.EventLog.BombPlant, false, ("Misc"), ("LogPlant"));
	SetupValue(g_Options.Misc.EventLog.BombDefuse, false, ("Misc"), ("LogDefuse"));
	SetupValue(g_Options.Misc.EventLog.AbortPlant, false, ("Misc"), ("LogAbortPlant"));
	SetupValue(g_Options.Misc.EventLog.Tick, false, ("Misc"), ("LogTick"));

	SetupValue(g_Options.Misc.FakeMedia, false, ("Misc"), ("FakeMedia"));
	SetupValue(g_Options.Misc.afkbot, false, ("Misc"), ("AFK-BOT"));

	SetupValue(g_Options.Misc.inventoryalwayson, false, ("Misc"), ("EnableInventory"));

	SetupValue(g_Options.Misc.radaringame, false, ("Misc"), ("Radar-Minimap"));
	SetupValue(g_Options.Misc.radarwindow, false, ("Misc"), ("Radar-Window"));

	SetupValue(g_Options.Misc.radrzoom, false, ("Misc"), ("Radar-Zoom"));
	SetupValue(g_Options.Misc.radralpha, 0.f, ("Misc"), ("Radar-Alpha"));
	SetupValue(g_Options.Misc.radrsize, 0, ("Misc"), ("Radar-Size"));

	SetupValue(g_Options.Skinchanger.Enabled, false, ("skins"), ("enable"));
	SetupValue(g_Options.Skinchanger.gloveskin, 0, ("skins"), ("gloveskin"));
	SetupValue(g_Options.Skinchanger.glove, 0, ("skins"), ("glove"));
	SetupValue(g_Options.Skinchanger.knifemodel, 0, ("skins"), ("modelknife"));
	for (int id = 0; id < 65; id++) {
		SetupValue(g_Options.Skinchanger.W[id].ChangerSkin, 0, GetWeaponName(id), ("Skin"));
		//		SetupValue(g_Options.Skinchanger.W[id].ChangerName, chicken, GetWeaponName(id), ("Name"));
		SetupValue(g_Options.Skinchanger.W[id].ChangerStatTrak, 0, GetWeaponName(id), ("StatTrak"));
		SetupValue(g_Options.Skinchanger.W[id].ChangerWear, 0.f, GetWeaponName(id), ("Wear"));
		SetupValue(g_Options.Skinchanger.W[id].ChangerSeed, 0, GetWeaponName(id), ("Seed"));
		SetupValue(g_Options.Skinchanger.W[id].ChangerQuality, 0, GetWeaponName(id), ("Quality"));
	}

	for (int id = 500; id <= 516; id++) { // knife skins

		if ((id < 505 && id >= 501) || (id > 512 && id < 514))
			continue;

		SetupValue(g_Options.Skinchanger.W[id].ChangerSkin, 0, GetWeaponName(id), ("Skin"));
		//		SetupValue(g_Options.Skinchanger.W[id].ChangerName[32], skinName, GetWeaponName(id), ("Name"));
		SetupValue(g_Options.Skinchanger.W[id].ChangerStatTrak, 0, GetWeaponName(id), ("StatTrak"));
		SetupValue(g_Options.Skinchanger.W[id].ChangerWear, 0.f, GetWeaponName(id), ("Wear"));
		SetupValue(g_Options.Skinchanger.W[id].ChangerSeed, 0, GetWeaponName(id), ("Seed"));
		SetupValue(g_Options.Skinchanger.W[id].ChangerQuality, 0, GetWeaponName(id), ("Quality"));
	}

	//COLORS

	SetupValue(g_Options.Colors.TeamESP[0], 0.0f, "Colors", "TeamESP1");
	SetupValue(g_Options.Colors.TeamESP[1], 0.0f, "Colors", "TeamESP2");
	SetupValue(g_Options.Colors.TeamESP[2], 0.0f, "Colors", "TeamESP3");

	SetupValue(g_Options.Colors.EnemyESP[0], 0.0f, "Colors", "EnemyESP1");
	SetupValue(g_Options.Colors.EnemyESP[1], 0.0f, "Colors", "EnemyESP2");
	SetupValue(g_Options.Colors.EnemyESP[2], 0.0f, "Colors", "EnemyESP3");

	SetupValue(g_Options.Colors.EnemyChamsNVis[0], 0.0f, "Colors", "EnemyChamsInVis1");
	SetupValue(g_Options.Colors.EnemyChamsNVis[1], 0.0f, "Colors", "EnemyChamsInVis2");
	SetupValue(g_Options.Colors.EnemyChamsNVis[2], 0.0f, "Colors", "EnemyChamsInVis3");

	SetupValue(g_Options.Colors.EnemyChamsVis[0], 0.0f, "Colors", "EnemyChamsVis1");
	SetupValue(g_Options.Colors.EnemyChamsVis[1], 0.0f, "Colors", "EnemyChamsVis2");
	SetupValue(g_Options.Colors.EnemyChamsVis[2], 0.0f, "Colors", "EnemyChamsVis3");

	SetupValue(g_Options.Colors.TeamChamsNVis[0], 0.0f, "Colors", "TeamChamsInVis1");
	SetupValue(g_Options.Colors.TeamChamsNVis[1], 0.0f, "Colors", "TeamChamsInVis2");
	SetupValue(g_Options.Colors.TeamChamsNVis[2], 0.0f, "Colors", "TeamChamsInVis3");

	SetupValue(g_Options.Colors.TeamChamsVis[0], 0.0f, "Colors", "TeamChamsVis1");
	SetupValue(g_Options.Colors.TeamChamsVis[1], 0.0f, "Colors", "TeamChamsVis2");
	SetupValue(g_Options.Colors.TeamChamsVis[2], 0.0f, "Colors", "TeamChamsVis3");

	SetupValue(g_Options.Colors.GrenadeBounce[0], 0.0f, "Colors", "GrenadeBounce1");
	SetupValue(g_Options.Colors.GrenadeBounce[1], 0.0f, "Colors", "GrenadeBounce2");
	SetupValue(g_Options.Colors.GrenadeBounce[2], 0.0f, "Colors", "GrenadeBounce3");

	SetupValue(g_Options.Colors.GrenadeLine[0], 0.0f, "Colors", "GrenadeLine1");
	SetupValue(g_Options.Colors.GrenadeLine[1], 0.0f, "Colors", "GrenadeLine2");
	SetupValue(g_Options.Colors.GrenadeLine[2], 0.0f, "Colors", "GrenadeLine3");

	SetupValue(g_Options.Colors.OffscreenIndicator[0], 0.0f, "Colors", "Offscreen1");
	SetupValue(g_Options.Colors.OffscreenIndicator[1], 0.0f, "Colors", "Offscreen2");
	SetupValue(g_Options.Colors.OffscreenIndicator[2], 0.0f, "Colors", "Offscreen3");

	SetupValue(g_Options.Colors.HeadPosition[0], 0.0f, "Colors", "HeadPosition1");
	SetupValue(g_Options.Colors.HeadPosition[1], 0.0f, "Colors", "HeadPosition2");
	SetupValue(g_Options.Colors.HeadPosition[2], 0.0f, "Colors", "HeadPosition3");

	SetupValue(g_Options.Colors.Hitmarker[0], 0.0f, "Colors", "Hitmarker1");
	SetupValue(g_Options.Colors.Hitmarker[1], 0.0f, "Colors", "Hitmarker2");
	SetupValue(g_Options.Colors.Hitmarker[2], 0.0f, "Colors", "Hitmarker3");

	SetupValue(g_Options.Colors.Backtrack[0], 0.0f, "Colors", "backtrackdots_color1");
	SetupValue(g_Options.Colors.Backtrack[1], 0.0f, "Colors", "backtrackdots_color2");
	SetupValue(g_Options.Colors.Backtrack[2], 0.0f, "Colors", "backtrackdots_color3");

	SetupValue(g_Options.Colors.DeathHitbox[0], 0.0f, "Colors", "Hitbox1");
	SetupValue(g_Options.Colors.DeathHitbox[1], 0.0f, "Colors", "Hitbox2");
	SetupValue(g_Options.Colors.DeathHitbox[2], 0.0f, "Colors", "Hitbox3");

	SetupValue(g_Options.Colors.Skeleton[0], 0.0f, "Colors", "Skeleton1");
	SetupValue(g_Options.Colors.Skeleton[1], 0.0f, "Colors", "Skeleton2");
	SetupValue(g_Options.Colors.Skeleton[2], 0.0f, "Colors", "Skeleton3");

	SetupValue(g_Options.Colors.RecoilCrosshair[0], 0.0f, "Colors", "color_recoil1");
	SetupValue(g_Options.Colors.RecoilCrosshair[1], 0.0f, "Colors", "color_recoil2");
	SetupValue(g_Options.Colors.RecoilCrosshair[2], 0.0f, "Colors", "color_recoil3");

	SetupValue(g_Options.Colors.SpreadCrosshair[0], 0.0f, "Colors", "color_spread1");
	SetupValue(g_Options.Colors.SpreadCrosshair[1], 0.0f, "Colors", "color_spread2");
	SetupValue(g_Options.Colors.SpreadCrosshair[2], 0.0f, "Colors", "color_spread3");

	SetupValue(g_Options.Colors.SniperCrosshair[0], 0.0f, "Colors", "color_sniper1");
	SetupValue(g_Options.Colors.SniperCrosshair[1], 0.0f, "Colors", "color_sniper2");
	SetupValue(g_Options.Colors.SniperCrosshair[2], 0.0f, "Colors", "color_sniper3");

	SetupValue(g_Options.Colors.GlowEnemy[0], 0.0f, "Colors", "glow_enemy1");
	SetupValue(g_Options.Colors.GlowEnemy[1], 0.0f, "Colors", "glow_enemy2");
	SetupValue(g_Options.Colors.GlowEnemy[2], 0.0f, "Colors", "glow_enemy3");

	SetupValue(g_Options.Colors.GlowTeam[0], 0.0f, "Colors", "glow_team1");
	SetupValue(g_Options.Colors.GlowTeam[1], 0.0f, "Colors", "glow_team2");
	SetupValue(g_Options.Colors.GlowTeam[2], 0.0f, "Colors", "glow_team3");

	SetupValue(g_Options.Colors.FakeAngleChams[0], 0.0f, "Colors", "FakeAngleChams1");
	SetupValue(g_Options.Colors.FakeAngleChams[1], 0.0f, "Colors", "FakeAngleChams2");
	SetupValue(g_Options.Colors.FakeAngleChams[2], 0.0f, "Colors", "FakeAngleChams3");

	SetupValue(g_Options.Colors.HandChams[0], 0.0f, "Colors", "HandsColor1");
	SetupValue(g_Options.Colors.HandChams[1], 0.0f, "Colors", "HandsColor2");
	SetupValue(g_Options.Colors.HandChams[2], 0.0f, "Colors", "HandsColor3");

	SetupValue(g_Options.Colors.WeaponChams[0], 0.0f, "Colors", "WeaponColor1");
	SetupValue(g_Options.Colors.WeaponChams[1], 0.0f, "Colors", "WeaponColor2");
	SetupValue(g_Options.Colors.WeaponChams[2], 0.0f, "Colors", "WeaponColor3");

	SetupValue(g_Options.Colors.AimLineColor[0], 0.0f, "Colors", "AimLineColor1");
	SetupValue(g_Options.Colors.AimLineColor[1], 0.0f, "Colors", "AimLineColor2");
	SetupValue(g_Options.Colors.AimLineColor[2], 0.0f, "Colors", "AimLineColor3");

	SetupValue(g_Options.Colors.ScopeLine[0], 0.0f, "Colors", "ScopeLineColor1");
	SetupValue(g_Options.Colors.ScopeLine[1], 0.0f, "Colors", "ScopeLineColor2");
	SetupValue(g_Options.Colors.ScopeLine[2], 0.0f, "Colors", "ScopeLineColor3");

	SetupValue(g_Options.Colors.HeadLine[0], 0.0f, "Colors", "HeadLine1");
	SetupValue(g_Options.Colors.HeadLine[1], 0.0f, "Colors", "HeadLine2");
	SetupValue(g_Options.Colors.HeadLine[2], 0.0f, "Colors", "HeadLine3");

	SetupValue(g_Options.Colors.BoxFillEnemy[0], 0.0f, "Colors", "BoxFillEnemy1");
	SetupValue(g_Options.Colors.BoxFillEnemy[1], 0.0f, "Colors", "BoxFillEnemy2");
	SetupValue(g_Options.Colors.BoxFillEnemy[2], 0.0f, "Colors", "BoxFillEnemy3");
	SetupValue(g_Options.Colors.BoxFillEnemy[3], 0.0f, "Colors", "BoxFillEnemy4");

	SetupValue(g_Options.Colors.BoxFillTeam[0], 0.0f, "Colors", "BoxFillTeam1");
	SetupValue(g_Options.Colors.BoxFillTeam[1], 0.0f, "Colors", "BoxFillTeam2");
	SetupValue(g_Options.Colors.BoxFillTeam[2], 0.0f, "Colors", "BoxFillTeam3");
	SetupValue(g_Options.Colors.BoxFillTeam[3], 0.0f, "Colors", "BoxFillTeam4");

	SetupValue(g_Options.Colors.DamageIndicator[0], 0.0f, "Colors", "DamageIndicator1");
	SetupValue(g_Options.Colors.DamageIndicator[1], 0.0f, "Colors", "DamageIndicator2");
	SetupValue(g_Options.Colors.DamageIndicator[2], 0.0f, "Colors", "DamageIndicator3");

	SetupValue(g_Options.Colors.DroppedWeapons[0], 0.0f, "Colors", "droppedguns1");
	SetupValue(g_Options.Colors.DroppedWeapons[1], 0.0f, "Colors", "droppedguns2");
	SetupValue(g_Options.Colors.DroppedWeapons[2], 0.0f, "Colors", "droppedguns3");

	SetupValue(g_Options.Colors.BulletTracer[0], 0.0f, "Colors", "BulletTracers_1");
	SetupValue(g_Options.Colors.BulletTracer[1], 0.0f, "Colors", "BulletTracers_2");
	SetupValue(g_Options.Colors.BulletTracer[2], 0.0f, "Colors", "BulletTracers_3");

	SetupValue(g_Options.Colors.AmbientLight[0], 0.0f, "Colors", "ambientlightcolor_1");
	SetupValue(g_Options.Colors.AmbientLight[1], 0.0f, "Colors", "ambientlightcolor_2");
	SetupValue(g_Options.Colors.AmbientLight[2], 0.0f, "Colors", "ambientlightcolor_3");
}

void CConfig::SetupValue(int &value, int def, std::string category, std::string name)
{
	value = def;
	ints.push_back(new ConfigValue<int>(category, name, &value));
}

void CConfig::SetupValue(float &value, float def, std::string category, std::string name)
{
	value = def;
	floats.push_back(new ConfigValue<float>(category, name, &value));
}

void CConfig::SetupValue(char &value, char def, std::string category, std::string name)
{
	value = def;
	chars.push_back(new ConfigValue<char>(category, name, &value));
}

void CConfig::SetupValue(bool &value, bool def, std::string category, std::string name)
{
	value = def;
	bools.push_back(new ConfigValue<bool>(category, name, &value));
}



void CConfig::Save(char* name)
{
	static TCHAR path[MAX_PATH];
	std::string folder, file;
	if (strlen(name) > 0)
	{
		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, 0, path)))
		{
			folder = std::string(path) + ("\\tornadonight\\");
			file = std::string(path) + ("\\tornadonight\\") + std::string(name) + XorStr(".tornadonight");
		}
	}
	else
	{
		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, 0, path)))
		{
			folder = std::string(path) + ("\\tornadonight\\");
			file = std::string(path) + ("\\tornadonight\\config.tornadonight");
		}
	}

	CreateDirectory(folder.c_str(), NULL);

	for (auto value : ints)
		WritePrivateProfileString(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : floats)
		WritePrivateProfileString(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : chars)
		WritePrivateProfileString(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : bools)
		WritePrivateProfileString(value->category.c_str(), value->name.c_str(), *value->value ? "true" : "false", file.c_str());
}

void CConfig::Load(char* name)
{
	static TCHAR path[MAX_PATH];
	std::string folder, file;
	if (strlen(name) > 0)
	{
		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, 0, path)))
		{
			folder = std::string(path) + ("\\tornadonight\\");
			file = std::string(path) + ("\\tornadonight\\") + std::string(name) + XorStr(".tornadonight");
		}
	}
	else
	{
		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, 0, path)))
		{
			folder = std::string(path) + ("\\tornadonight\\");
			file = std::string(path) + ("\\tornadonight\\config.tornadonight");
		}
	}

	char value_l[32] = { '\0' };

	for (auto value : ints)
	{
		GetPrivateProfileString(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = atoi(value_l);
	}

	for (auto value : floats)
	{
		GetPrivateProfileString(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = (float)atof(value_l);
	}

	for (auto value : chars)
	{
		GetPrivateProfileString(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = atoi(value_l);
	}

	for (auto value : bools)
	{
		GetPrivateProfileString(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = !strcmp(value_l, "true");
	}
}

CConfig* Config = new CConfig();
Variables g_Options;