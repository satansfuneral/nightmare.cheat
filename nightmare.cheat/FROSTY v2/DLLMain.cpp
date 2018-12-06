#include "Interfaces.h"
#include "Hooks.h"
#include "Render.h"
#include "SDK.h"
#include "recvproxy.h"
#include "Misc.h"
#include "Global.h"
#include <TlHelp32.h>
#include <winternl.h>
bool unload;
HMODULE hModule;

void ErasePE()
{
	char *pBaseAddr = (char*)GetModuleHandle(NULL);
	DWORD dwOldProtect = 0;
	VirtualProtect(pBaseAddr, 4096, PAGE_READWRITE, &dwOldProtect);
	ZeroMemory(pBaseAddr, 4096);
	VirtualProtect(pBaseAddr, 4096, dwOldProtect, &dwOldProtect);
}

bool on_dll_attach(void* base)
{
	while (!GetModuleHandleA("serverbrowser.dll"))
		Sleep(100);

	InitialiseInterfaces();
	g_Netvars->GetNetvars();
	g_Render->SetupFonts();
	hooks::initialize();	
	NetvarHook();

	while (unload == false)
	{
		Sleep(1000);
	}

	UnloadProxy();
	hooks::cleanup();
	ErasePE();

	Sleep(2000);

	FreeLibraryAndExitThread((HMODULE)base, 0);
}

bool on_dll_detach()
{
	UnloadProxy();
	hooks::cleanup();
	ErasePE();
	return 1;
}


BOOL WINAPI DllMain(
	_In_      HINSTANCE hinstDll,
	_In_      DWORD     fdwReason,
	_In_opt_	LPVOID    lpvReserved
)
{
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hinstDll);
		CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)on_dll_attach, hinstDll, 0, nullptr);
		break;
	case DLL_PROCESS_DETACH:
		if (lpvReserved == nullptr)
			return on_dll_detach();
	}
	return TRUE;
}