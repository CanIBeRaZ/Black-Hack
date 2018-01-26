#include "main.h"
#include "Reflective Loader\ReflectiveLoader.h"

HMODULE hAppInstance;

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
		break;
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

