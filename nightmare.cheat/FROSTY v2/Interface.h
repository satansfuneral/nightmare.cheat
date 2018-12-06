#pragma once
#include "Configuration.hpp"
#include "dropboxes.h"
#include "Variables.h"
#include <cctype>
#include "Listener.h"
#include "SpecList.h"
#include "namespammer.h"
#include "Radar.h"
#include "memoryfonts.h"
#include "SkinParser.h"
#include "SkinFilter.h"
#include "EventLog.h"
#include "MiscOptions.h"
#include <vector>


#define RandomInt(nMin, nMax) (rand() % (nMax - nMin + 1) + nMin);
#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))
#define BUILDSTAMP ( __DATE__ )

//========Clantag====================

void SetClantag(const char *tag)
{
	static auto fnClantagChanged = (int(__fastcall*)(const char*, const char*))U::pattern_scan(GetModuleHandleA("engine.dll"), "53 56 57 8B DA 8B F9 FF 15");

	fnClantagChanged(tag, tag);
}


bool IsUtility(ItemDefinitionIndexx index)
{
	switch (index)
	{
	case ItemDefinitionIndexx::ITEM_NONE:
	case ItemDefinitionIndexx::WEAPON_C4:
	case ItemDefinitionIndexx::WEAPON_FLASH:
	case ItemDefinitionIndexx::WEAPON_HE:
	case ItemDefinitionIndexx::WEAPON_INC:
	case ItemDefinitionIndexx::WEAPON_MOLOTOV:
	case ItemDefinitionIndexx::WEAPON_SMOKE:
	case ItemDefinitionIndexx::WEAPON_DECOY:
	case ItemDefinitionIndexx::WEAPON_TASER:
	case ItemDefinitionIndexx::WEAPON_KNIFE_T:
	case ItemDefinitionIndexx::WEAPON_KNIFE_CT:
	case ItemDefinitionIndexx::GLOVE_T_SIDE:
	case ItemDefinitionIndexx::GLOVE_CT_SIDE:
	case ItemDefinitionIndexx::GLOVE_SPORTY:
	case ItemDefinitionIndexx::GLOVE_SLICK:
	case ItemDefinitionIndexx::GLOVE_LEATHER_WRAP:
	case ItemDefinitionIndexx::GLOVE_STUDDED_BLOODHOUND:
	case ItemDefinitionIndexx::GLOVE_MOTORCYCLE:
	case ItemDefinitionIndexx::GLOVE_SPECIALIST:
	case ItemDefinitionIndexx::GLOVE_HYDRA:
		return true;
	default:
		return false;
	}
}

bool Contains(const std::string &word, const std::string &sentence) {
	if (word == "" || sentence == "")
		return true;

	return sentence.find(word) != std::string::npos;
}

std::string ToLower(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), (int(*)(int))std::tolower);

	return str;
}
//--------------------------------------------
bool defaultk;
bool bayonet;
bool flip;
bool gut;
bool karambit;
bool m9bayonet;
bool huntsman;
bool falchion;
bool bowie;
bool butterfly;
bool daggers;

void doknives(bool* disknife)
{
	defaultk = false;
	bayonet = false;
	flip = false;
	gut = false;
	karambit = false;
	m9bayonet = false;
	huntsman = false;
	falchion = false;
	bowie = false;
	butterfly = false;
	daggers = false;

	*disknife = true;
}



void originalbuttons()
{
	auto& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Button] = ImColor(21, 21, 21, 255);
	style.Colors[ImGuiCol_ButtonHovered] = ImColor(21, 21, 21, 255);
	style.Colors[ImGuiCol_ButtonActive] = ImColor(21, 21, 21, 255);
}

void bordernew()
{
	auto& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Border] = ImVec4(0.27f, 0.27f, .27f, 1.00f);
}



void originalchild()
{
	auto& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_ChildWindowBg] = ImColor(10, 10, 10, 255);
	style.ChildRounding = 0.0f;
}

int ParseSkins()
{
	valve_parser::Document doc;
	auto r = doc.Load(".\\csgo\\scripts\\items\\items_game.txt", valve_parser::ENCODING::UTF8);
	if (!r)
		return 1;

	valve_parser::Document english;
	r = english.Load(".\\csgo\\resource\\csgo_english.txt", valve_parser::ENCODING::UTF16_LE);
	if (!r)
		return 2;

	auto weaponSkinCombo = doc.BreadthFirstSearch("weapon_icons");
	if (!weaponSkinCombo || !weaponSkinCombo->ToObject())
		return 3;

	auto skinDataVec = doc.BreadthFirstSearchMultiple("paint_kits");
	if (!skinDataVec.size())
		return 4;

	auto PaintKitNames = english.BreadthFirstSearch("Tokens");
	if (!PaintKitNames || !PaintKitNames->ToObject())
		return 5;

	//std::unordered_map<std::string, std::set<std::string>> G::weaponSkins;
	//std::unordered_map<std::string, skinInfo> G::skinMap;
	//std::unordered_map<std::string, std::string> G::skinNames;

	std::vector<std::string> weaponNames = {
		"deagle",
		"elite",
		"fiveseven",
		"glock",
		"ak47",
		"aug",
		"awp",
		"famas",
		"g3sg1",
		"galilar",
		"m249",
		"m4a1_silencer", //needs to be before m4a1 else silencer doesnt get filtered out :D
		"m4a1",
		"mac10",
		"p90",
		"ump45",
		"xm1014",
		"bizon",
		"mag7",
		"negev",
		"sawedoff",
		"tec9",
		"hkp2000",
		"mp7",
		"mp9",
		"nova",
		"p250",
		"scar20",
		"sg556",
		"ssg08",
		"usp_silencer",
		"cz75a",
		"revolver",
		"knife_m9_bayonet", //needs to be before bayonet else knife_m9_bayonet doesnt get filtered out :D
		"bayonet",
		"knife_flip",
		"knife_gut",
		"knife_karambit",
		"knife_tactical",
		"knife_falchion",
		"knife_survival_bowie",
		"knife_butterfly",
		"knife_push",
		"studded_bloodhound_gloves",
		"sporty_gloves",
		"slick_gloves",
		"leather_handwraps",
		"motorcycle_gloves",
		"specialist_gloves"

	};

	//populate G::weaponSkins
	for (auto child : weaponSkinCombo->children)
	{
		if (child->ToObject())
		{
			for (auto weapon : weaponNames)
			{
				auto skinName = child->ToObject()->GetKeyByName("icon_path")->ToKeyValue()->Value.toString();
				auto pos = skinName.find(weapon);
				//filter out the skinname
				if (pos != std::string::npos)
				{
					auto pos2 = skinName.find_last_of('_');
					g_Options.Skinchanger.weaponSkins[weapon].insert(
						skinName.substr(pos + weapon.length() + 1,
							pos2 - pos - weapon.length() - 1)
					);
					break;
				}
			}
		}
	}

	//populate skinData
	for (auto skinData : skinDataVec)
	{
		if (skinData->ToObject())
		{
			for (auto skin : skinData->children)
			{
				if (skin->ToObject())
				{
					skinInfo si;
					si.paintkit = skin->ToObject()->name.toInt();

					auto skinName = skin->ToObject()->GetKeyByName("name")->ToKeyValue()->Value.toString();
					auto tagNode = skin->ToObject()->GetKeyByName("description_tag");
					if (tagNode)
					{
						std::string tag = tagNode->ToKeyValue()->Value.toString();
						tag = tag.substr(1, std::string::npos); //skip #
						std::transform(tag.begin(), tag.end(), tag.begin(), towlower);
						si.tagName = tag;
					}

					auto keyVal = skin->ToObject()->GetKeyByName("seed");
					if (keyVal != nullptr)
						si.seed = keyVal->ToKeyValue()->Value.toInt();

					g_Options.Skinchanger.skinMap[skinName] = si;
				}
			}
		}
	}

	//populate G::skinNames
	for (auto child : PaintKitNames->children)
	{
		if (child->ToKeyValue())
		{
			std::string key = child->ToKeyValue()->Key.toString();
			std::transform(key.begin(), key.end(), key.begin(), towlower);
			if (key.find("paintkit") != std::string::npos &&
				key.find("tag") != std::string::npos)
			{
				g_Options.Skinchanger.skinNames[key] = child->ToKeyValue()->Value.toString();
			}
		}
	}

	return 0;
}



void initializeskins()
{
	static bool once = false;

	if (!once)
	{
		ParseSkins();
		once = true;
	}
}

namespace prometheus
{


	void GUI_Init(HWND window, IDirect3DDevice9 *pDevice)
	{

		if (ImGui_ImplDX9_Init(window, pDevice))
		{

			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();

			
			




			ImGuiStyle& style = ImGui::GetStyle();


			static int hue = 140;
			ImVec4 col_text = ImColor::HSV(hue / 255.f, 20.f / 255.f, 235.f / 255.f);
			ImVec4 col_theme = ImColor(17, 17, 17);
			ImVec4 col_main = ImColor(9, 82, 128);
			ImVec4 col_back = ImColor(35, 35, 35);
			ImVec4 col_area = ImColor(4, 32, 41);

			style.Colors[ImGuiCol_Text] = ImVec4(0.98f, 0.98f, 0.98f, 1.00f);
			style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
			style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.99f);
			style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.07f, 0.07f, 0.07f, 1.f);
			style.Colors[ImGuiCol_Border] = ImVec4(0.27f, 0.27f, .27f, 1.00f);
			style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			style.Colors[ImGuiCol_FrameBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.f); //testing
			style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.13, 0.13, 0.13, 1.f);
			style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.11, 0.11, 0.11, 1.f);
			style.Colors[ImGuiCol_TitleBg] = ImVec4(col_theme.x, col_theme.y, col_theme.z, .97f);
			style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(col_theme.x, col_theme.y, col_theme.z, .97f);
			style.Colors[ImGuiCol_TitleBgActive] = ImVec4(col_theme.x, col_theme.y, col_theme.z, .97f);
			style.Colors[ImGuiCol_MenuBarBg] = ImVec4(.52f, 0.f, 0.52f, .7f);
			style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.95f);
			style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f); //main half
			style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.50f, 0.50f, 0.50f, 0.70f); //main half
			style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(.30f, 0.f, 0.1f, 2.f);
			style.Colors[ImGuiCol_CheckMark] = ImVec4(.50f, 0.f, 0.f, .87f);
			style.Colors[ImGuiCol_CheckMark] = ImVec4(0.5f, 0.f, 0.1f, 2.f);
			style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f); //main half
			style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.5f, 0.f, 0.1f, 2.f);
			style.Colors[ImGuiCol_Button] = ImVec4(0.20, 0.20, 0.20, 1.f);
			style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.20, 0.20, 0.20, 1.f);
			style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.20, 0.20, 0.20, 1.f);
			style.Colors[ImGuiCol_Header] = ImVec4(.35f, 0.35f, 0.35f, .3f);
			style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.5f, 0.f, 0.1f, 2.f); // combobox hover
			style.Colors[ImGuiCol_HeaderActive] = ImVec4(.30f, 0.f, 0.1f, 2.f);
			style.Colors[ImGuiCol_Column] = ImVec4(col_text.x, col_text.y, col_text.z, 0.32f);
			style.Colors[ImGuiCol_ColumnHovered] = ImVec4(col_text.x, col_text.y, col_text.z, 0.78f);
			style.Colors[ImGuiCol_ColumnActive] = ImVec4(col_text.x, col_text.y, col_text.z, 1.00f);
			style.Colors[ImGuiCol_ResizeGrip] = ImVec4(col_main.x, col_main.y, col_main.z, 0.20f);
			style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 0.78f);
			style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
			style.Colors[ImGuiCol_CloseButton] = ImVec4(col_text.x, col_text.y, col_text.z, 0.f);
			style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(col_text.x, col_text.y, col_text.z, 0);
			style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(col_text.x, col_text.y, col_text.z, 0);
			style.Colors[ImGuiCol_PlotLines] = ImVec4(col_text.x, col_text.y, col_text.z, 0.63f);
			style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
			style.Colors[ImGuiCol_PlotHistogram] = ImVec4(col_text.x, col_text.y, col_text.z, 0.63f);
			style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
			style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(col_main.x, col_main.y, col_main.z, 0.43f);
			style.Colors[ImGuiCol_PopupBg] = ImVec4(col_main.x, col_main.y, col_main.z, 0.92f);
			style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);



			style.WindowPadding = ImVec2(8, 8);
			style.WindowMinSize = ImVec2(32, 32);
			style.WindowRounding = 0.5f;
			style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
			//style.ChildRounding = 0.0f;
			style.FramePadding = ImVec2(4, 2);
			style.FrameRounding = 0.0f;

			style.ItemSpacing = ImVec2(8, 4);
			style.ItemInnerSpacing = ImVec2(4, 4);
			style.TouchExtraPadding = ImVec2(0, 0);
			style.IndentSpacing = 21.0f;
			style.ColumnsMinSpacing = 3.0f;
			style.ScrollbarSize = 12.0f;
			style.ScrollbarRounding = 0.0f;
			style.GrabMinSize = 7.0f;
			style.GrabRounding = 3.0f;
			style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
			style.DisplayWindowPadding = ImVec2(22, 22);
			style.DisplaySafeAreaPadding = ImVec2(4, 4);
			style.AntiAliasedLines = true;
			style.CurveTessellationTol = 1.25f;


			G::d3dinit = true;
		}

	}




	void mainWindow()
	{

		C_BaseEntity* pLocal = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());

		const char* aaset[] = { "aimbot", "triggerbot" };
		const char* weapon[] = { "rifles", "pistols","snipers", "smg", "lmg" };
		const char* knife[] = { "deafult", "bayonet","flip", "gut", "karambit", "m9", "huntsman", "falchion", "bowie", "butterfly", "daggers" };
		const char* glove[] = { "default", "bloodhound", "sport", "driver", "handwraps", "motorcycle", "specialist", "hydra"};
		


		auto& style = ImGui::GetStyle();
		static int page = 1;
		ImGui::SetNextWindowSize(ImVec2(500.f, 310.f));

		ImGui::SetNextWindowSize(ImVec2(500, 310));
		ImGui::Begin("nightmare", &g_Options.Menu.Opened, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize );
		{

			bordernew();
			originalbuttons();
			static int tab_count = 0;
			//ImGui::BeginChild("##tabs", ImVec2(150, 0), true, 0);
			//{

			const char* tabs[] = { "     aimbot", "     visuals", "     player", "     misc", "     skins","     config","     changelog" };


			ImGui::PushItemWidth(140);
			ImGui::ListBox("##tablist", &tab_count, tabs, ARRAYSIZE(tabs), 16);
			ImGui::PopItemWidth();

			ImGui::SameLine();
			ImGui::BeginChild("##tabs2", ImVec2(0, 0), true, 0);
			{
				switch (tab_count)
				{
				case 0:
				{

					
					
					ImGui::Text("aimbot", ImVec2(0, 0));
					ImGui::Separator();


						


					


				}
				break;
				case 1:
				{

				}
				break;
				case 2:
				{

				}
				break;
				case 3:
				{

				}
				break;
				case 4:
				{

					static char filterWeapons[32];
					static char filterSkins[32];

					
					


						ImGui::PushItemWidth(155);
						ImGui::InputText(("##FILTERWEAPONS"), filterWeapons, IM_ARRAYSIZE(filterWeapons));
						ImGui::SameLine();
						ImGui::PushItemWidth(150);
						ImGui::InputText(("##FILTERSKINS"), filterSkins, IM_ARRAYSIZE(filterSkins));
						ImGui::PopItemWidth();
						ImGui::ListBoxHeader(("##GUNS"), ImVec2(155, 175));
						for (auto it : ItemDefinitionIndexxMap)
						{
							bool isDefault = (int)it.first < 0;
							if (!isDefault && !Contains(ToLower(std::string(filterWeapons)), ToLower(ItemDefinitionIndexxMap.at(it.first).entityName)))
								continue;

							if (IsUtility(it.first))
								continue;

							const bool item_selected = ((int)it.first == (int)g_Options.Menu.currentWeapon);

							std::string formattedName;

							formattedName = ItemDefinitionIndexxMap.at(it.first).entityName;
							if (ImGui::Selectable(formattedName.c_str(), item_selected))
							{
								g_Options.Menu.currentWeapon = (int)it.first;
							}

						}
						ImGui::ListBoxFooter();
					
					ImGui::SameLine();

					
						std::string skinName = weaponnames(g_Options.Menu.currentWeapon);
						{
							
							ImGui::PopItemWidth();
							ImGui::ListBoxHeader(("##SKINS"), ImVec2(150, 175));
							std::string skinStr = "";
							int curItem = -1;
							int s = 0;
							for (auto skin : g_Options.Skinchanger.weaponSkins[skinName])
							{
								int pk = g_Options.Skinchanger.skinMap[skin].paintkit;
								if (pk == g_Options.Skinchanger.W[g_Options.Menu.currentWeapon].ChangerSkin)
									curItem = s;
								bool selected = g_Options.Skinchanger.W[g_Options.Menu.currentWeapon].ChangerSkin == pk;


								skinStr = g_Options.Skinchanger.skinNames[g_Options.Skinchanger.skinMap[skin].tagName].c_str();
								skinStr += " | ";
								skinStr += std::to_string(g_Options.Skinchanger.skinMap[skin].paintkit);
								if (!Contains(ToLower(std::string(filterSkins)), ToLower(skinStr)))
									continue;
								if (ImGui::Selectable(skinStr.c_str(), selected))
								{
									pk = g_Options.Skinchanger.skinMap[skin].paintkit;
									g_Options.Skinchanger.W[g_Options.Menu.currentWeapon].ChangerSkin = pk;
									U::FullUpdate();
									break;
								}
								s++;
							}

							ImGui::ListBoxFooter();
						}
						ImGui::PushItemWidth(155);
						ImGui::Combo((""), &g_Options.Skinchanger.knifemodel, knife, ARRAYSIZE(knife));
						
						
						 ImGui::SameLine();
						 ImGui::Combo(("##Type"), &g_Options.Skinchanger.glove, glove, ARRAYSIZE(glove));

						 if (ImGui::Button(("Apply##skinchanger"), ImVec2(155, 15)))
						 {
							 U::FullUpdate();
						 }

						 ImGui::SameLine();

						 const char* gstr;
						 if (g_Options.Skinchanger.glove == 1)
						 {
							 gstr = "charred\0\rsnakebite\0\rBronzed\0\rguerilla\0\0";
						 }
						 else if (g_Options.Skinchanger.glove == 2)
						 {
							 gstr = "hedge maze\0\rpandoras box\0\rsuperconductor\0\rarid\0\vice\0\omega\0\amphibious\0\bronze morph\0";
						 }
						 else if (g_Options.Skinchanger.glove == 3)
						 {
							 gstr = "lunar weave\0\rconvoy\0\rcrimson weave\0\rdiamondback\0\overtake\0\racing green\0\king snake\0\imperial plaid\0";
						 }
						 else if (g_Options.Skinchanger.glove == 4)
						 {
							 gstr = "leather\0\rspruce ddpat\0\rslaughter\0\rbadlands\0\cobalt skulls\0\overprint\0\duct tape\0\arboreal\0";
						 }
						 else if (g_Options.Skinchanger.glove == 5)
						 {
							 gstr = "Eclipse\0\rSpearmint\0\rBoom!\0\rCool Mint\0\Turtle\0\Transport\0\Polygon\0\POW!\0";
						 }
						 else if (g_Options.Skinchanger.glove == 6)
						 {
							 gstr = "Forest DDPAT\0\rCrimson Kimono\0\rEmerald Web\0\rFoundation\0\Crimson Web\0\Buckshot\0\Fade\0\Mogul\0";
						 }
						 else if (g_Options.Skinchanger.glove == 7)
						 {
							 gstr = "Case Hardened\0\rRattler\0\rMangrove\0\rEmerald\0\0";
						 }
						 else
						 {
							 gstr = "";
						 }

						 if (ImGui::Combo(("##2"), &g_Options.Skinchanger.gloveskin, gstr, -1))
							 U::FullUpdate();

						 ImGui::Checkbox(("id list"), &g_Options.Skinchanger.SkinFilter);
						 ImGui::SameLine();
						 ImGui::Checkbox(("misc options"), &g_Options.Skinchanger.MiscSkinOptions);

				}
				break;
				case 5:
				{

				}
				break;
				case 6:
				{
					ImGui::Text("+added skinchanger");
					ImGui::Text("+reworked menu");
				}
				break;
				}
			}
			ImGui::EndChild();



			

		}ImGui::End();

	}
}