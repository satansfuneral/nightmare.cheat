#pragma once
#include "Vector.h"
#include <vector>

namespace Globals
{
	extern int Shots;
	extern int missedshots;
	extern float RealAngle;
	extern float FakeAngle;
	extern Vector AimPoint;
	extern bool shouldflip;
	extern bool ySwitch;
	extern float NextTime;
	extern int resolverModes[64];
	extern float fakeAngle;
	extern float OldSimulationTime[65];
	extern bool error;
	extern bool pFakewalk;
	extern bool bForceFullUpdate;
	extern float Hitchance;
	extern bool bSendPacketG;
}