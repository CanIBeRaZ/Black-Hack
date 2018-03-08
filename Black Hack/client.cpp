#include "main.h"





void CL_CreateMove(float frametime, struct usercmd_s *cmd, int active)
{
	g_Client.CL_CreateMove(frametime, cmd, active);


}

void HUD_Frame(double time)
{

	g_Client.HUD_Frame(time);
}

void HUD_Frame_init(double time)
{
	HookOpenGL();

	g_Engine.Con_Printf((char*)charenc("Injected :3\n"));

	g_pClient->HUD_Frame = HUD_Frame;

	g_Client.HUD_Frame(time);
}

void HookClient()
{
	g_pClient->HUD_Frame = HUD_Frame_init;
	g_pClient->CL_CreateMove = CL_CreateMove;
}