struct Module
{
	DWORD base;
	DWORD size;
	DWORD end;
};

class COffsets;
class COffsets
{
public:
	Module hardware, client, gameui, vgui2;

	bool FindOffsets(void);
	bool FindModuleByName(const char *moduleName, Module *module);
	void Error(PCHAR msg);
	DWORD Absolute(DWORD Address);
	DWORD GetModuleSize(DWORD Address);
	DWORD FindReference(DWORD start, DWORD end, DWORD Address);
	DWORD FindPush(DWORD start, DWORD end, PCHAR Message);
	DWORD FindPattern(PCHAR pattern, DWORD start, DWORD end, DWORD offset);
	DWORD FindPattern(PCHAR pattern, PCHAR mask, DWORD start, DWORD end, DWORD offset);
	DWORD FarProc(const DWORD Address, DWORD LB, DWORD HB);
	PVOID FindClient(void);
	PVOID FindEngine(void);
};
extern COffsets g_Offsets;

extern cl_clientfunc_t *g_pClient;
extern cl_clientfunc_t g_Client;
extern cl_enginefunc_t *g_pEngine;
extern cl_enginefunc_t g_Engine;