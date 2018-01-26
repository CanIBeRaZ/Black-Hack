#include "main.h"
#include "Reflective Loader\ReflectiveLoader.h"

HMODULE hAppInstance;

DWORD WINAPI Hook(LPVOID lpThreadParameter)
{
	MessageBoxA(NULL, charenc("Hello from DllMain!"), charenc("Reflective Dll Injection"), MB_OK);//Testing

	return NULL;
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
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Hook, NULL, NULL, NULL);
		break;
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

