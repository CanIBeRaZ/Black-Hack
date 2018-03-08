#include "main.h"

COffsets g_Offsets;

cl_clientfunc_t *g_pClient = nullptr;
cl_clientfunc_t g_Client;
cl_enginefunc_t *g_pEngine = nullptr;
cl_enginefunc_t g_Engine;

bool COffsets::FindOffsets(void)
{
	if (!FindModuleByName(charenc("hw.dll"), &hardware))
		return false;

	if (!FindModuleByName(charenc("client.dll"), &client))
		return false;

	if (!FindModuleByName(charenc("gameui.dll"), &gameui))
		return false;

	if (!FindModuleByName(charenc("vgui2.dll"), &vgui2))
		return false;

	g_pClient = (cl_clientfunc_t*)FindClient();
	g_pEngine = (cl_enginefunc_t*)FindEngine();

	while (!g_Client.V_CalcRefdef)
		RtlCopyMemory(&g_Client, g_pClient, sizeof(cl_clientfunc_t));

	while (!g_Engine.V_CalcShake)
		RtlCopyMemory(&g_Engine, g_pEngine, sizeof(cl_enginefunc_t));

	return true;
}

PVOID COffsets::FindEngine(void)
{
	DWORD Address = *(PDWORD)(FindPush(client.base, client.end, (PCHAR)charenc("sprites/voiceicon.spr")) + 0x07);

	if (FarProc(Address, client.base, client.end))
		Error((PCHAR)charenc("Engine: not found."));

	return (PVOID)Address;
}

PVOID COffsets::FindClient(void)
{
	DWORD Address = FindPattern((PCHAR)charenc("ScreenFade"), hardware.base, hardware.end, 0);
	PVOID Pointer = (PVOID)*(PDWORD)(FindReference(hardware.base, hardware.end, Address) + 0x13); //all patch

	if (FarProc((DWORD)Pointer, hardware.base, hardware.end))
		Error((PCHAR)charenc("Client: not found."));

	return Pointer;
}

DWORD COffsets::Absolute(DWORD Address)
{
	return Address + *(PDWORD)Address + 4;
}

DWORD COffsets::FindReference(DWORD start, DWORD end, DWORD Address)
{
	char szPattern[] = { 0x68 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 };
	*(PDWORD)&szPattern[1] = Address;
	return FindPattern(szPattern, start, end, 0);
}

DWORD COffsets::FindPush(DWORD start, DWORD end, PCHAR Message)
{
	char bPushAddrPattern[] = { 0x68 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 };
	DWORD Address = FindPattern(Message, start, end, 0);
	*(PDWORD)&bPushAddrPattern[1] = Address;
	Address = FindPattern((PCHAR)bPushAddrPattern, start, end, 0);
	return Address;
}

DWORD COffsets::FarProc(const DWORD Address, DWORD LB, DWORD HB)
{
	return ((Address < LB) || (Address > HB));
}

DWORD COffsets::GetModuleSize(DWORD Address)
{
	return PIMAGE_NT_HEADERS(Address + (DWORD)PIMAGE_DOS_HEADER(Address)->e_lfanew)->OptionalHeader.SizeOfImage;
}

void COffsets::Error(PCHAR msg)
{
	MessageBoxA(NULL, msg, charenc("FATAL ERROR"), MB_OK | MB_ICONERROR);
	TerminateProcess(GetCurrentProcess(), 0);
}

bool COffsets::FindModuleByName(const char *moduleName, Module *module)
{
	if (!moduleName || !*moduleName || !module)
		return false;

	HMODULE hModuleDll = GetModuleHandle(moduleName);

	if (!hModuleDll)
		return false;

	module->base = (DWORD)hModuleDll;
	module->size = GetModuleSize(module->base);
	module->end = module->base + module->size - 1;

	return true;
}

DWORD COffsets::FindPattern(PCHAR pattern, PCHAR mask, DWORD start, DWORD end, DWORD offset)
{
	if (start > end)
	{
		DWORD reverse = end;
		end = start;
		start = reverse;
	}

	size_t patternLength = strlen(pattern);
	bool found = false;

	for (DWORD i = start; i < end - patternLength; i++)
	{
		found = true;

		for (size_t idx = 0; idx < patternLength; idx++)
		{
			if (mask[idx] == 'x' && pattern[idx] != *(PCHAR)(i + idx))
			{
				found = false;
				break;
			}
		}

		if (found)
			return i + offset;
	}

	return 0;
}

DWORD COffsets::FindPattern(PCHAR pattern, DWORD start, DWORD end, DWORD offset)
{
	if (start > end)
	{
		DWORD reverse = end;
		end = start;
		start = reverse;
	}

	size_t patternLength = strlen(pattern);
	bool found = false;

	for (DWORD i = start; i < end - patternLength; i++)
	{
		found = true;

		for (size_t idx = 0; idx < patternLength; idx++)
		{
			if (pattern[idx] != *(PCHAR)(i + idx))
			{
				found = false;
				break;
			}
		}

		if (found)
			return i + offset;
	}

	return 0;
}