#include "HookIncludes.h"
#include "HooksList.h"
#include "Listener.h"

bool spoofcheck = false;

void spoof()
{
	if (spoofcheck)
		return;

	static auto sv_cheats = g_CVar->FindVar("sv_cheats");
	static auto spoof_cheats = new SpoofedConvar(sv_cheats);
	spoof_cheats->SetInt(1);

	spoofcheck = true;
}

namespace hooks
{
	vfunc_hook panel;
	vfunc_hook client;
	vfunc_hook clientmode;
	vfunc_hook prediction;
	vfunc_hook surface;
	vfunc_hook eventmanager;
	vfunc_hook viewrender;
	vfunc_hook engine;
	vfunc_hook d3d;

	void initialize()
	{
		client.setup(g_CHLClient);
		client.hook_index(37, hkFrameStageNotify);
		client.hook_index(22, hkCreateMove);


		clientmode.setup(g_ClientMode);
		clientmode.hook_index(18, hkOverrideView);
		clientmode.hook_index(35, hkGetViewModelFOV);

		panel.setup(g_Panel);
		panel.hook_index(41, hkPaintTraverse);

		//		modelrender.setup(g_ModelRender);
		//		modelrender.hook_index(21, hkDrawModelExecute);


		viewrender.setup(g_RenderView);
		viewrender.hook_index(9, hkSceneEnd);


		surface.setup(g_Surface);
		surface.hook_index(82, hkPlaySound);
		surface.hook_index(67, hkUnlockCursor);

		engine.setup(g_Engine);
		engine.hook_index(27, IsConnected);

		initializeskins();

		spoof();

		window = FindWindow("Valve001", NULL);
		G::OldWindow = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)Handlers::Hooked_WndProc);

		if (g_D3DDevice9)
		{
			d3d.setup(g_D3DDevice9);
			d3d.hook_index(16, Handlers::Hooked_Reset);
			d3d.hook_index(42, Handlers::Hooked_EndScene);
		}

		/*        m_present = U::pattern_scan(GetModuleHandleW(L"gameoverlayrenderer.dll"), "FF 15 ? ? ? ? 8B F8 85 DB 74 1F") + 0x2;//big ( large ) obs bypass
		m_reset = U::pattern_scan(GetModuleHandleW(L"gameoverlayrenderer.dll"), "FF 15 ? ? ? ? 8B F8 85 FF 78 18") + 0x2;  //big ( large ) obs bypass



		oPresent = **reinterpret_cast<Present_T**>(m_present);
		oReset = **reinterpret_cast<Reset_t**>(m_reset);

		**reinterpret_cast<void***>(m_present) = reinterpret_cast<void*>(&hkPresent);
		**reinterpret_cast<void***>(m_reset) = reinterpret_cast<void*>(&hkReset);*/

		item_purchase::singleton()->initialize();

		if (!spoofcheck)
		{
			MessageBox(NULL, "The spoofing process failed!", "TornadoNight", MB_OK);
			exit(0);
		}

		//		PlaySoundA(opening, NULL, SND_ASYNC | SND_MEMORY);
	}

	void cleanup()
	{
		item_purchase::singleton()->remove();
		client.unhook_all();
		prediction.unhook_all();
		clientmode.unhook_all();
		panel.unhook_all();
		surface.unhook_all();
		viewrender.unhook_all();
		engine.unhook_all();
		d3d.unhook_all();
		Glow::ClearGlow();
	}

}