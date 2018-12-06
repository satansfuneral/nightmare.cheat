#pragma once
#include "Globals.h"

namespace Globals
{
	int Shots;
	int missedshots;
	float RealAngle;
	float FakeAngle;
	Vector AimPoint;
	bool shouldflip;
	bool ySwitch;
	float NextTime;
	int resolverModes[64];
	float fakeAngle;
	float OldSimulationTime[65];
	bool error;
	bool pFakewalk = false;
	bool bForceFullUpdate = true;
	float Hitchance;
	bool bSendPacketG;
}