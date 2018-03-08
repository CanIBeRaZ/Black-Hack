#include "main.h"

void CL_CreateMove(float frametime, struct usercmd_s *cmd, int active)
{
	g_Client.CL_CreateMove(frametime, cmd, active);


}

void HookClient()
{
	g_pClient->CL_CreateMove = CL_CreateMove;
}