#pragma once
#include "SDK.h"

class CEnginePred
{
public:
	void RunPrediction(CInput::CUserCmd * cmd);
	void EndPrediction(CInput::CUserCmd * cmd);
};

extern CEnginePred* g_EnginePred;