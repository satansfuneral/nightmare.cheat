#pragma once

const char* opt_GlowStylesPlayer[] = { "Outline outer", "Cover", "Outline inner" };

const char* opt_GlowStylesOther[] = { "Outline outer", "Cover", "Outline inner" };

const char* Hitmarker[] = { "OFF", "Basic", "Advanced" };

const char* Hitsound[] = { "OFF", "COD", "Metal", "Bolbi", "Bubble", "Thunk", "Click" };

const char* opt_Chams[] = { "Textured", "Textured XQZ", "Flat", "Flat XQZ" };

const char* fakelag[] = 
{
	"Velocity1",
	"Velocity2"
};
const char* hitscan[] =
{
	"OFF",
	"Low",
	"Medium",
	"High",
	"BAIM"
};
const char* fakewalktype[] =
{
	"OFF",
	"Fakewalk",
	"Lagwalk"
};
const char* Resolvers[] = 
{
	"OFF",
	"LBY",
	"LBY 2",
	"Alternative",
	"Legit AA",
	"Nospread"
};

const char* antiaimyawtrue[] =
{
	"Off",
	"Sideways Jitter",
	"Slow Spin",
	"Fast Spin",
	"Backwards",
	"Fake Head", // 5
	"Manual LBY Break",
	"Back Jitter",
	"Jitter LBY Break",
	"Freestanding"
};

const char* antiaimyawtruemove[] =
{
	"Off",
	"Sideways Jitter",
	"Slow Spin",
	"Fast Spin",
	"Backwards",
	"Fake Head", //5
	"Manual AA",
	"Back Jitter",
	"Freestanding"
};


const char* antiaimyawfake[] =
{
	"Off",
	"Sideways Jitter",
	"Slow Spin",
	"Fast Spin",
	"Backwards",
	"LBY Switch",
	"Manual LBY Break",
	"Small Jitter",
	"Jitter AA",
	"Fake Head", //9
	"local view"
};

const char* antiaimyawfakemove[] =
{
	"Off",
	"Sideways Jitter",
	"Slow Spin",
	"Fast Spin",
	"Backwards",
	"LBY Switch",
	"Manual LBY Break",
	"Jitter",
	"Jitter AA",
	"fake head" //9
};

const char* antiaimpitch[] =
{
	"Off",
	"Down",
	"Up",
	"Fake Up",
	"Fake Down",
	"fake head"
};

const char* aimBones[] =
{
	"Head",
	"Neck",
	"Pelvis"
};

const char* autostop[] =
{
	"OFF",
	"Stop",
	"Fakewalk"
};

const char* offscreencheck[] = 
{
	"OFF",
	"World To Screen",
	"Visibility",
	"Smart"
};
const char* HandsMode[] =
{
	"OFF",
	"No Hands",
	"Chams",
	"Wireframe",
	"Gold",
	"Plastic",
	"Glass",
	"Crystal",
	"Platinum"
};

const char* chamswp[] =
{
	"OFF",
	"Chams",
	"Wireframe",
	"Gold",
	"Plastic",
	"Glass",
	"Crystal",
	"Platinum"
};

const char* skyboxchanger[] =
{
	"OFF",
	"Baggage",
	"Tibet",
	"Clear Sky",
	"Clear Sky HD",
	"Embassy",
	"Italy",
	"Daylight 1",
	"Daylight 2",
	"Daylight 3",
	"Daylight 4",
	"Cloudy",
	"Night 1",
	"Night 2",
	"Night Flat",
	"Day HD",
	"Day",
	"Black",
	"Vertigo HD",
	"Vertigo Blue HD",
	"Vertigo",
	"Vietnam",
	"Dusty Sky",
	"Jungle",
	"Nuke",
	"Office"
};
const char* HitmarkSound[] =
{
	"Disabled",
	"Default",
	"Roblox",
	"Trump",
	"Metallic",
	"Bolbi",
	"Bubble",
	"Thunk"
};
const char* Backtracktype[] =
{
	"off",
	"dots",
	"skeleton"
};

const char* headpostype[] = 
{
	"OFF",
	"Rectangle",
	"Circle",
	"3D Box"
};

const char* snipercrosshair[] =
{
	"OFF",
	"Line|Static",
	"Line|A-Wall"
};

const char* recoil[] =
{
	"OFF",
	"Minimal",
	"None"
};





const char* smoke[] =
{
	"OFF",
	"Wired",
	"Invisible"
};

const char* grenades[] =
{
	"OFF",
	"Text",
	"Icons"
};

const char* healthtype[] =
{
	"Normal",
	"Edgy"
};

const char* skeletonopts[] =
{
	"Normal",
	"HP-Based"
};

const char* rccrosshair[] =
{
	"OFF",
	"Cross",
	"Circle",
	"Engine"
};

/*const char* configFiles[] =
{
"Legit",
"Rage",
"HvH",
"Other"
}; */


const char* droppedWeapons[] =
{
	"OFF",
	"Text",
	"Icons"
};

const char* WeaponType[] =
{
	"OFF",
	"Text",
	"Icons"
};

const char* autostrafe[] =
{
	"OFF",
	"Legit",
	"Rage"
};


const char* spammerlist[] =
{
	"OFF",
	"Tornadonight",
	"Premium CSGO"
};

const char* keyNames[] =
{
	"",
	"Mouse 1",
	"Mouse 2",
	"Cancel",
	"Middle Mouse",
	"Mouse 4",
	"Mouse 5",
	"",
	"Backspace",
	"Tab",
	"",
	"",
	"Clear",
	"Enter",
	"",
	"",
	"Shift",
	"Control",
	"Alt",
	"Pause",
	"Caps",
	"",
	"",
	"",
	"",
	"",
	"",
	"Escape",
	"",
	"",
	"",
	"",
	"Space",
	"Page Up",
	"Page Down",
	"End",
	"Home",
	"Left",
	"Up",
	"Right",
	"Down",
	"",
	"",
	"",
	"Print",
	"Insert",
	"Delete",
	"",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"",
	"",
	"",
	"",
	"",
	"Numpad 0",
	"Numpad 1",
	"Numpad 2",
	"Numpad 3",
	"Numpad 4",
	"Numpad 5",
	"Numpad 6",
	"Numpad 7",
	"Numpad 8",
	"Numpad 9",
	"Multiply",
	"Add",
	"",
	"Subtract",
	"Decimal",
	"Divide",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12",

};


const char* weaponnames(int id)
{
	switch (id)
	{
	case 1:
		return "deagle";
	case 2:
		return "elite";
	case 3:
		return "fiveseven";
	case 4:
		return "glock";
	case 7:
		return "ak47";
	case 8:
		return "aug";
	case 9:
		return "awp";
	case 10:
		return "famas";
	case 11:
		return "g3sg1";
	case 13:
		return "galilar";
	case 14:
		return "m249";
	case 60:
		return "m4a1_silencer";
	case 16:
		return "m4a1";
	case 17:
		return "mac10";
	case 19:
		return "p90";
	case 24:
		return "ump45";
	case 25:
		return "xm1014";
	case 26:
		return "bizon";
	case 27:
		return "mag7";
	case 28:
		return "negev";
	case 29:
		return "sawedoff";
	case 30:
		return "tec9";
	case 32:
		return "hkp2000";
	case 33:
		return "mp7";
	case 34:
		return "mp9";
	case 35:
		return "nova";
	case 36:
		return "p250";
	case 38:
		return "scar20";
	case 39:
		return "sg556";
	case 40:
		return "ssg08";
	case 61:
		return "usp_silencer";
	case 63:
		return "cz75a";
	case 64:
		return "revolver";
	case 508:
		return "knife_m9_bayonet";
	case 500:
		return "bayonet";
	case 505:
		return "knife_flip";
	case 506:
		return "knife_gut";
	case 507:
		return "knife_karambit";
	case 509:
		return "knife_tactical";
	case 512:
		return "knife_falchion";
	case 514:
		return "knife_survival_bowie";
	case 515:
		return "knife_butterfly";
	case 516:
		return "knife_push";

	default:
		return "";
	}
}