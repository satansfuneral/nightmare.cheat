#pragma once
#include "Entities.h"

int C_BaseEntity::GetNumAnimOverlays()
{
	return *(int*)((DWORD)this + 0x297C);
}

AnimationLayer *C_BaseEntity::GetAnimOverlays()
{
	// to find offset: use 9/12/17 dll
	// sig: 55 8B EC 51 53 8B 5D 08 33 C0
	return *(AnimationLayer**)((DWORD)this + 0x2970);
}

AnimationLayer *C_BaseEntity::GetAnimOverlay(int i)
{
	if (i < 15)
		return &GetAnimOverlays()[i];
}

int C_BaseEntity::GetSequenceActivity(int sequence)
{
	auto hdr = g_MdlInfo->GetStudiomodel(this->GetModel());

	if (!hdr)
		return -1;

	// sig for stuidohdr_t version: 53 56 8B F1 8B DA 85 F6 74 55
	// sig for C_BaseAnimating version: 55 8B EC 83 7D 08 FF 56 8B F1 74 3D
	// c_csplayer vfunc 242, follow calls to find the function.

	static auto get_sequence_activity = reinterpret_cast<int(__fastcall*)(void*, studiohdr_t*, int)>(getSequenceActivity);

	return get_sequence_activity(this, hdr, sequence);
}

C_CSGOPlayerAnimState *C_BaseEntity::GetPlayerAnimState()
{
	return (C_CSGOPlayerAnimState*)((uintptr_t)this + 0x3884); // 0x3870
}

/* every csgo activity */
enum CSGO_ACTS {
	ACT_CSGO_DEFUSE = 958,
	ACT_CSGO_DEFUSE_WITH_KIT = 959,
	ACT_CSGO_FLASHBANG_REACTION = 960,
	ACT_CSGO_FIRE_PRIMARY = 961,
	ACT_CSGO_FIRE_PRIMARY_OPT_1 = 962,
	ACT_CSGO_FIRE_PRIMARY_OPT_2 = 963,
	ACT_CSGO_FIRE_SECONDARY = 964, // shooting secondary
	ACT_CSGO_RELOAD = 967,
	ACT_CSGO_RELOAD_START = 968,
	ACT_CSGO_RELOAD_LOOP = 969,
	ACT_CSGO_RELOAD_END = 970,
	ACT_CSGO_PLANT_BOMB = 978,
	ACT_CSGO_IDLE_TURN_BALANCEADJUST = 979,
	ACT_CSGO_IDLE_ADJUST_STOPPEDMOVING = 980,
};

class CAnimationLayer
{
public:
	char  pad_0000[20];
	uint32_t m_nOrder; //0x0014
	uint32_t m_nSequence; //0x0018
	float_t m_flPrevCycle; //0x001C
	float_t m_flWeight; //0x0020
	float_t m_flWeightDeltaRate; //0x0024
	float_t m_flPlaybackRate; //0x0028
	float_t m_flCycle; //0x002C
	void *m_pOwner; //0x0030 // player's thisptr
	char  pad_0038[4]; //0x0034
}; //Size: 0x0038