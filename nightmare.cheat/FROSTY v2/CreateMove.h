#pragma once
#include "HookIncludes.h"
#include "Misc.h"
#include "RageBot.h"
#include "LegitBot.h"
#include "GrenadePrediction.h"
#include "LagComp.h"
#include "ESP.h"
#include "OverrideView.h"
#include "LocalInfo.h"
#include "EnginePrediction.h"

using create_move_t = void(__thiscall *)(IBaseClientDLL *, int, float, bool);
QAngle qLastTickAngles = QAngle(0.0f, 0.0f, 0.0f);
void __stdcall CHLCreateMove(int sequence_number, float input_sample_frametime, bool active, bool& bSendPacket)
{
	static auto ofunc = hooks::client.get_original<create_move_t>(22);
	ofunc(g_CHLClient, sequence_number, input_sample_frametime, active);
	C_BaseEntity *local = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
	CInput::CUserCmd *cmd = g_Input->GetUserCmd(0, sequence_number);

	if (!cmd)
		return;

	PVOID pebp;
	__asm mov pebp, ebp;
	bool* pbSendPacket = (bool*)(*(DWORD*)pebp - 0x1C);
	bSendPacket = *pbSendPacket;

	static float next_time = 0;

	if (local->GetVelocity().Length2D() < 0.1)
	{
		float TickStuff = TICKS_TO_TIME(local->GetTickBase());

		Globals::shouldflip = false;

		bool moveFlip = false;

		Globals::NextTime = next_time;

		if (next_time - TICKS_TO_TIME(local->GetTickBase()) > 1.1)
			next_time = 0;

		if (local->GetVelocity().Length2D() > 0.1)
			moveFlip = true;

		if (local->GetVelocity().Length2D() < 0.1 && !moveFlip)
		{
			if (TickStuff > next_time + 1.1f)
			{
				next_time = TickStuff + TICKS_TO_TIME(1);

				Globals::shouldflip = true;
			}
		}

		else if (moveFlip && local->GetVelocity().Length2D() < 0.1)
		{
			if (TickStuff > next_time + 0.22f)
			{
				next_time = TickStuff + TICKS_TO_TIME(1);

				Globals::shouldflip = true;

				moveFlip = false;
			}
		}
	}

	if (!cmd->command_number)
		return;

	CInput::CVerifiedUserCmd *verifiedCommands = *(CInput::CVerifiedUserCmd **)(reinterpret_cast< uint32_t >(g_Input) + 0xF0);
	CInput::CVerifiedUserCmd *verified = &verifiedCommands[sequence_number % 150];
	if (!verified)
		return;

	if (!local)
		return;
	QAngle oldAngle = cmd->viewangles;
	QAngle oldangles;
	float oldForward = cmd->forwardmove;
	float oldSideMove = cmd->sidemove;
	
	if (g_Options.Misc.ServerRankRevealAll)
		RankReveal(cmd);

	switch (g_Options.Misc.AutoStrafe)
	{
	case 1:
		misc::LegitAutoStrafe(cmd, local, cmd->viewangles);
		break;
	case 2:
		misc::RageAutoStrafe(cmd, local, cmd->viewangles);
		break;
	}
	int FakeWalkKey = g_Options.Ragebot.fakewalkkey;
	if (g_Options.Ragebot.walktype == 1 && FakeWalkKey > 0 && G::PressedKeys[g_Options.Ragebot.fakewalkkey] && g_Options.Ragebot.AutoStop != 3)
	{
		ragebot::FakeWalk(cmd, bSendPacket);
	}
	if (g_Options.Ragebot.walktype == 2)
	{
		ragebot::LagWalk(cmd, bSendPacket);
	}


	misc::instance().OnCreateMove(cmd, local);

	CCSGrenadeHint::instance().Tick(cmd->buttons);

	if (g_Options.Ragebot.EnginePrediction)
		g_EnginePred->RunPrediction(cmd);

	legitbot::instance().OnCreateMove(cmd, local, bSendPacket);
	backtracking->legitBackTrack(cmd, local);

	ragebot::instance().OnCreateMove(cmd, bSendPacket);
	backtracking->RageBackTrack(cmd, local);

	if (g_Options.Ragebot.EnginePrediction)
		g_EnginePred->EndPrediction(cmd);

	if (!bSendPacket && g_Options.Ragebot.YawFake) qLastTickAngles = cmd->viewangles;// 
	else if (bSendPacket && !g_Options.Ragebot.YawFake) qLastTickAngles = cmd->viewangles; // 

	if (g_Options.Misc.antiuntrusted)
		if (!sanitize_angles(cmd->viewangles))
			return;

	movementfix(oldAngle, cmd);

	clampMovement(cmd);

	verified->m_cmd = *cmd;
	verified->m_crc = cmd->GetChecksum();

	if (Globals::ySwitch) {
		Globals::FakeAngle = cmd->viewangles.y;
	}
	else {
		Globals::RealAngle = cmd->viewangles.y;
	}

}
#pragma warning(disable : 4409)
__declspec(naked) void __stdcall hkCreateMove(int sequence_number, float input_sample_frametime, bool active)
{
	__asm
	{
		PUSH	EBP
		MOV		EBP, ESP
		PUSH	EBX
		LEA		ECX, [ESP]
		PUSH	ECX
		PUSH	active
		PUSH	input_sample_frametime
		PUSH	sequence_number
		CALL	CHLCreateMove
		POP		EBX
		POP		EBP
		RETN	0xC
	}
}