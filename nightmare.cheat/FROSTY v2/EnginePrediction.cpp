#include "EnginePrediction.h"

float m_flOldCurtime;
float m_flOldFrametime;
CMoveData m_MoveData;
int* m_pPredictionRandomSeed;

void CEnginePred::RunPrediction(CInput::CUserCmd* cmd)
{
	C_BaseEntity *pLocal = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
	static bool bInit = false;
	if (!bInit) {
		m_pPredictionRandomSeed = *(int**)(U::FindPattern(U::ClientModule(), (PBYTE)"\x8B\x0D\x00\x00\x00\x00\xBA\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x83\xC4\x04", "xx????x????x????xxx") + 2);
		bInit = true;
	}


	*m_pPredictionRandomSeed = cmd->random_seed;


	m_flOldCurtime = g_Globals->curtime;
	m_flOldFrametime = g_Globals->frametime;

	g_Globals->curtime = pLocal->GetTickBase() * g_Globals->interval_per_tick;
	g_Globals->frametime = g_Globals->interval_per_tick;

	g_GameMovement->StartTrackPredictionErrors(pLocal);

	memset(&m_MoveData, 0, sizeof(m_MoveData));
	g_MoveHelper->SetHost(pLocal);
	g_Prediction->SetupMove(pLocal, cmd, g_MoveHelper, &m_MoveData);
	g_GameMovement->ProcessMovement(pLocal, &m_MoveData);
	g_Prediction->FinishMove(pLocal, cmd, &m_MoveData);
}

void CEnginePred::EndPrediction(CInput::CUserCmd * cmd)
{
	C_BaseEntity *pLocal = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
	g_GameMovement->FinishTrackPredictionErrors(pLocal);
	g_MoveHelper->SetHost(0);

	*m_pPredictionRandomSeed = -1;

	g_Globals->curtime = m_flOldCurtime;
	g_Globals->frametime = m_flOldFrametime;
}

CEnginePred* g_EnginePred;