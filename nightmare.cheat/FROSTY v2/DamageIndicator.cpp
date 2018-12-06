#include "DamageIndicator.h"
DamageIndicators damage_indicators;
void DamageIndicators::paint() {

	Color color(int(g_Options.Colors.DamageIndicator[0] * 255.f), int(g_Options.Colors.DamageIndicator[1] * 255.f), int(g_Options.Colors.DamageIndicator[2] * 255.f), 255.f);

	auto m_local = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());

	float current_time = m_local->GetTickBase() * g_Globals->interval_per_tick;

	for (int i = 0; i < data.size(); i++) {
		if (data[i].flEraseTime < current_time) {
			data.erase(data.begin() + i);
			continue;
		}

		if (!data[i].bInitialized) {
			data[i].Position = data[i].Player->GetHeadPos();
			data[i].bInitialized = true;
		}

		if (current_time - data[i].flLastUpdate > 0.0001f) {
			data[i].Position.z -= (0.1f * (current_time - data[i].flEraseTime));
			data[i].flLastUpdate = current_time;
		}

		Vector screen_pos;
		int pLineSize = 8;
		if (g_Render->WorldToScreen(data[i].Position, screen_pos)) {
			g_Render->Textf(screen_pos.x, screen_pos.y, color, g_Render->font.ESP, std::to_string(data[i].iDamage).c_str());
		}
	}
}