#pragma once

#include <windows.h>

bool RemoveHeader(HMODULE hModule)
{
	DWORD dwStartOffset = (DWORD)hModule;

	IMAGE_DOS_HEADER *pDosHeader = (PIMAGE_DOS_HEADER)dwStartOffset;

	if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
		return false;

	IMAGE_NT_HEADERS *pNtHeader = (PIMAGE_NT_HEADERS)(dwStartOffset + pDosHeader->e_lfanew);

	if (pNtHeader->Signature != IMAGE_NT_SIGNATURE)
		return false;

	DWORD dwOldProtection = NULL;
	if (!VirtualProtect((PVOID)hModule, pNtHeader->OptionalHeader.SizeOfHeaders, PAGE_EXECUTE_READWRITE, &dwOldProtection))
		return false;

	ZeroMemory((PVOID)hModule, pNtHeader->OptionalHeader.SizeOfHeaders);
	VirtualProtect((PVOID)hModule, pNtHeader->OptionalHeader.SizeOfHeaders, dwOldProtection, &dwOldProtection);

	return true;
}

bool HideThread(HANDLE hThread)
{
	typedef NTSTATUS(NTAPI *pNtSetInformationThread)(HANDLE, UINT, PVOID, ULONG);

	NTSTATUS Status;

	pNtSetInformationThread NtSIT = (pNtSetInformationThread)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtSetInformationThread");

	if (!NtSIT)
		return false;

	Status = hThread ? NtSIT(hThread, 0x11, 0, 0) : NtSIT(GetCurrentThread(), 0x11, 0, 0);

	return Status == 0;
}