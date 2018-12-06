#include "Interfaces.h"
#include "Utilities.h"

void InitialiseInterfaces()
{
	auto EnginePointer =	get_module_factory(GetModuleHandleA("engine.dll"));
	auto ClientPointer =	get_module_factory(GetModuleHandleA(U::ClientModule()));
	auto VGUIPointer =		get_module_factory(GetModuleHandleA("vguimatsurface.dll"));
	auto VGUI2Pointer =		get_module_factory(GetModuleHandleA("vgui2.dll"));
	auto MaterialPointer =	get_module_factory(GetModuleHandleA("materialsystem.dll"));
	auto PhysicsPointer =	get_module_factory(GetModuleHandleA("vphysics.dll"));
	auto StdPointer =		get_module_factory(GetModuleHandleA("vstdlib.dll"));
	auto CachePointer =		get_module_factory(GetModuleHandleA("datacache.dll"));
	auto DX9API =			GetModuleHandleA("shaderapidx9.dll");
	auto client =			GetModuleHandleA(U::ClientModule());

	getSequenceActivity =	(DWORD)U::pattern_scan(client, "55 8B EC 83 7D 08 FF 56 8B F1 74 3D");
	g_CHLClient =			(IBaseClientDLL*)ClientPointer("VClient018", nullptr);
	g_Engine =				(IVEngineClient*)EnginePointer("VEngineClient014", nullptr);
	g_EngineSound =			Interface::InterfaceManager::GetInterface<IEngineSound>("engine.dll", "IEngineSoundClient");
	g_Panel =				(IPanel*)VGUI2Pointer("VGUI_Panel009", nullptr);
	g_Surface =				(ISurface*)VGUIPointer("VGUI_Surface031", nullptr);
	g_EntityList =			(C_BaseEntityList*)ClientPointer("VClientEntityList003", nullptr);
	g_DebugOverlay =		(IVDebugOverlay*)EnginePointer("VDebugOverlay004", nullptr);
	g_Prediction =			(IPrediction*)ClientPointer("VClientPrediction001", nullptr);
	g_MaterialSystem =		(CMaterialSystem*)MaterialPointer("VMaterialSystem080", nullptr);
	g_RenderView =			(CVRenderView*)EnginePointer("VEngineRenderView014", nullptr);
	g_ModelRender =			(IVModelRender*)EnginePointer("VEngineModel016", nullptr);
	g_ModelInfo =			(CModelInfo*)EnginePointer("VModelInfoClient004", nullptr);
	g_MdlInfo =				Interface::InterfaceManager::GetInterface<IVModelInfoClient>("engine.dll", "VModelInfoClient");
	g_EngineTrace =			(IEngineTrace*)EnginePointer("EngineTraceClient004", nullptr);
	g_PhysProps =			(IPhysicsSurfaceProps*)PhysicsPointer("VPhysicsSurfaceProps001", nullptr);
	g_CVar =				(ICVar*)StdPointer("VEngineCvar007", nullptr);
	g_Dlight =				(IVEffects*)EnginePointer("VEngineEffects001", nullptr);
	g_GameMovement =		(IGameMovement*)ClientPointer("GameMovement001", nullptr);
	g_MoveHelper =			**(IMoveHelper***)(U::FindPattern(U::ClientModule(), (PBYTE)"\x8B\x0D\x00\x00\x00\x00\x8B\x46\x08\x68", "xx????xxxx") + 2);
	g_EventManager =		(IGameEventManager2*)EnginePointer("GAMEEVENTSMANAGER002", nullptr);
	g_GameConsole =			(IGameConsole*)ClientPointer("GameConsole004", nullptr);
	g_Input =				*(CInput**)(U::pattern_scan(GetModuleHandleA(U::ClientModule()), "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10") + 1);
	g_InputSystem =			Interface::InterfaceManager::GetInterface<IInputSystem>("inputsystem.dll", "InputSystemVersion");
	g_ViewRender =			*(IViewRender**)(U::FindPattern(U::ClientModule(), (PBYTE)"\xA1\x00\x00\x00\x00\xB9\x00\x00\x00\x00\xC7\x05\x00\x00\x00\x00\x00\x00\x00\x00\xFF\x10", "x????x????xx????????xx") + 1);
	g_PlayerResource =		**(C_CSPlayerResource***)(U::FindPattern(U::ClientModule(), (PBYTE)"\x8B\x3D\x00\x00\x00\x00\x85\xFF\x0F\x84\x00\x00\x00\x00\x81\xC7", "xx????xxxx????xx") + 2);
	g_GameRules =			**(C_CSGameRules***)(U::FindPattern(U::ClientModule(), (PBYTE)"\xA1\x00\x00\x00\x00\x8B\x0D\x00\x00\x00\x00\x6A\x00\x68\x00\x00\x00\x00\xC6\x80", "x????xx????xxx????xx") + 1);
	g_MdlCache =			(IMDLCache*)CachePointer("MDLCache004", nullptr);
	g_ChatElement =			FindHudElement<CHudChat>("CHudChat");
	g_ClientMode =			**(IClientMode***)((*(DWORD**)g_CHLClient)[10] + 0x5); // 8B 35 ? ? ? ? 8B 80 ] + 2
	g_Globals =				**(CGlobalVarsBase***)((*(DWORD**)g_CHLClient)[0] + 0x1B);
	g_pViewRenderBeams =	*reinterpret_cast< IViewRenderBeams** > (U::FindSig((U::ClientModule()), ("A1 ? ? ? ? 56 8B F1 B9 ? ? ? ? FF 50 08")) + 0x1); // 8D 43 FC B9 ? ? ? ? 50 A1 ] + 4
	g_pMemAlloc =			*(IMemAlloc**)(GetProcAddress(GetModuleHandle("tier0.dll"), "g_pMemAlloc"));
	g_D3DDevice9 =			**(IDirect3DDevice9***)(U::pattern_scan(DX9API, "A1 ? ? ? ? 50 8B 08 FF 51 0C") + 1);
	g_GlowObjManager =		*(CGlowObjectManager**)(U::pattern_scan(client, "0F 11 05 ? ? ? ? 83 C8 01") + 3); // A1 ? ? ? ? A8 01 75 4B
	g_StringTable =			(CNetworkStringTableContainer*)EnginePointer("VEngineClientStringTable001", NULL);
}

DWORD getSequenceActivity = 0x00;

IDirect3DDevice9* g_D3DDevice9 = nullptr;
IBaseClientDLL* g_CHLClient;
IVEngineClient* g_Engine;
IPanel* g_Panel;
C_BaseEntityList* g_EntityList;
ISurface* g_Surface;
IVDebugOverlay* g_DebugOverlay;
IClientMode* g_ClientMode;
CGlobalVarsBase* g_Globals;
IPrediction *g_Prediction;
CMaterialSystem* g_MaterialSystem;
CVRenderView* g_RenderView;
IVModelRender* g_ModelRender;
CModelInfo* g_ModelInfo;
IVModelInfoClient *g_MdlInfo;
IEngineTrace* g_EngineTrace;
IEngineSound* g_EngineSound;
IPhysicsSurfaceProps* g_PhysProps;
ICVar* g_CVar;
IVEffects* g_Dlight;
IMoveHelper* g_MoveHelper;
IGameMovement* g_GameMovement;
CInput* g_Input;
IInputSystem* g_InputSystem;
IGameEventManager2* g_EventManager;
C_CSPlayerResource* g_PlayerResource;
C_CSGameRules* g_GameRules;
IViewRender* g_ViewRender;
IGameConsole* g_GameConsole;
IMDLCache* g_MdlCache;
CHudChat* g_ChatElement;
CGlowObjectManager* g_GlowObjManager;
IViewRenderBeams* g_pViewRenderBeams;
IMemAlloc* g_pMemAlloc;
CNetworkStringTableContainer* g_StringTable;
CBaseClientState** g_ClientState;