#include "main.h"
#include "Reflective Loader\ReflectiveLoader.h"
#include "module protection\PEB.h"

HMODULE hAppInstance;

DWORD WINAPI Hook(LPVOID lpThreadParameter)
{
	CreateInterfaceFn gameui_factory = CaptureFactory(charenc("gameui.dll"));
	CreateInterfaceFn vgui2_factory = CaptureFactory(charenc("vgui2.dll"));
	CreateInterfaceFn hardware_factory = CaptureFactory(charenc("hw.dll"));
	CreateInterfaceFn client_factory = CaptureFactory(charenc("client.dll"));

	if (gameui_factory && vgui2_factory && hardware_factory && client_factory)
	{
		if (!g_Offsets.FindOffsets())
			g_Offsets.Error((PCHAR)charenc("No offsets found."));

		HookClient();

	}

	return TRUE;
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
    switch (ul_reason_for_call)
    {
	case DLL_QUERY_HMODULE:
		if (lpReserved != NULL)
			*(HMODULE *)lpReserved = hAppInstance;
		break;
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
		hAppInstance = hModule;

		DisableThreadLibraryCalls(hModule);

		if (RemoveHeader(hModule))
		{
			HANDLE hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Hook, NULL, NULL, NULL);

			if (!HideThread(hThread))
			{
				TerminateThread(hThread, 0);
				return FALSE;
			}
		}
		break;
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

