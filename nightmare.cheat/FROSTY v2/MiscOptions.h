#include "Render.h"

struct quality_name
{
	constexpr quality_name(const int index, const char* name) :
		index(index),
		name(name)
	{}

	int index = 0;
	const char* name = nullptr;
};

const std::vector<quality_name> k_quality_names =
{
{ 0, "Default" },
{ 1, "Genuine" },
{ 2, "Vintage" },
{ 3, "Unusual" },
{ 4, "UNKNOWN" },
{ 5, "Community" },
{ 6, "Developer" },
{ 7, "Self-Made" },
{ 8, "Customized" },
{ 9, "Strange" },
{ 10, "Completed" },
{ 11, "UNKNOWN2" },
{ 12, "Tournament" }
};

void MiscSkinOptions()
{
	ImGuiStyle& style = ImGui::GetStyle();

	style.Colors[ImGuiCol_TitleBg] = ImColor(21, 21, 21, 255);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImColor(21, 21, 21, 255);
	style.Colors[ImGuiCol_TitleBgActive] = ImColor(21, 21, 21, 255);

	if (ImGui::Begin("misc skin options", &g_Options.Skinchanger.MiscSkinOptions, ImVec2(245.f, 173.f), style.Alpha, ImGuiWindowFlags_NoCollapse))
	{
		ImGui::InputText("name", g_Options.Skinchanger.W[g_Options.Menu.currentWeapon].ChangerName, 32);
		ImGui::InputInt("stattrak", &g_Options.Skinchanger.W[g_Options.Menu.currentWeapon].ChangerStatTrak);
		ImGui::SliderFloat("wear", &g_Options.Skinchanger.W[g_Options.Menu.currentWeapon].ChangerWear, FLT_MIN, 1.f, "%.10f", 5);
		ImGui::InputInt("seed", &g_Options.Skinchanger.W[g_Options.Menu.currentWeapon].ChangerSeed);
		ImGui::Combo("quality", &g_Options.Skinchanger.W[g_Options.Menu.currentWeapon].ChangerQuality, [](void* data, int idx, const char** out_text)
		{
			*out_text = k_quality_names[idx].name;
			return true;
		}, nullptr, k_quality_names.size(), 5);
	}
}