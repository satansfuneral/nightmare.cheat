#include "Render.h"
#include "Sounds.h"

void EventLogOptions()
{
	ImGuiStyle& style = ImGui::GetStyle();

	style.Colors[ImGuiCol_TitleBg] = ImColor(21, 21, 21, 255);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImColor(21, 21, 21, 255);
	style.Colors[ImGuiCol_TitleBgActive] = ImColor(21, 21, 21, 255);

	if (ImGui::Begin("Event Log", &g_Options.Misc.EventLog.Enable, ImVec2(300.f, 310.f), style.Alpha, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
	{
		ImGui::BeginChild("##Options", ImVec2(400, 270), true);
		{
			ImGui::Text("Options");
			ImGui::Separator();
			ImGui::BeginChild(("#Options"), ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 21 * 5));
			{
				ImGui::Selectable((" Item Purchase"), &g_Options.Misc.EventLog.ItemPurchase);
				ImGui::Selectable((" Damage"), &g_Options.Misc.EventLog.Damage);
				ImGui::Selectable((" Bomb Plant"), &g_Options.Misc.EventLog.BombPlant);
				ImGui::Selectable((" Bomb Defuse"), &g_Options.Misc.EventLog.BombDefuse);
				ImGui::Selectable((" Abort Plant"), &g_Options.Misc.EventLog.AbortPlant);

			}ImGui::EndChild();

		}ImGui::EndChild();

		ImGui::End();
	}
}