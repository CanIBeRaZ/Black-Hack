#include "main.h"
#include "Reflective Loader\ReflectiveLoader.h"
#include "module protection\PEB.h"

HMODULE hAppInstance;

DWORD WINAPI Hook(LPVOID lpThreadParameter)
{
	MessageBoxA(NULL, charenc("Hello from DllMain!"), charenc("Reflective Dll Injection"), MB_OK);//Testing

	QAngle Test(123, 360, 99);
	Vector kek(12, 3, 0);
	Test.IsZero();

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

