#include "main.h"
#pragma optimize("", off)  
glBegin_t pglBegin = NULL;
glVertex3fv_t pglVertex3fv = NULL;
glColor4f_t pglColor4f = NULL;
wglSwapBuffers_t pwglSwapBuffers = NULL;
glClear_t pglClear = NULL;
glViewport_t pglViewport = NULL;

GLint viewport[4];

void APIENTRY Hooked_glBegin(GLenum mode)
{
	(*pglBegin)(mode);
}

void APIENTRY Hooked_glVertex3fv(GLfloat *v)
{
	(*pglVertex3fv)(v);
}

void APIENTRY Hooked_glColor4f(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	(*pglColor4f)(red, green, blue, alpha);
}

BOOL APIENTRY Hooked_wglSwapBuffers(HDC hdc)
{
	if (g_Engine.pfnGetCvarFloat((char*)"r_norefresh") == 0) {

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glOrtho(0, viewport[2], viewport[3], 0, 0, 1);
		glDisable(GL_DEPTH_TEST);
		glMatrixMode(GL_MODELVIEW);

		glPushMatrix();
		glLoadIdentity();
		glDisable(GL_TEXTURE_2D);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		g_Menu.Run();

		glDisable(GL_BLEND);

		glPopMatrix();

		glEnable(GL_TEXTURE_2D);
		glEnable(GL_DEPTH_TEST);
	}

	return(*pwglSwapBuffers)(hdc);
}

void APIENTRY Hooked_glClear(GLbitfield mask)
{
	if (mask == GL_DEPTH_BUFFER_BIT)
	{
		(*pglClear)(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	}

	(*pglClear)(mask);
}

void APIENTRY Hooked_glViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
	viewport[0] = x;
	viewport[1] = y;
	viewport[2] = width;
	viewport[3] = height;
	(*pglViewport)(x, y, width, height);
}

void HookOpenGL()
{
	HMODULE hmOpenGL = GetModuleHandle("opengl32.dll");
	pglBegin = (glBegin_t)DetourFunction((LPBYTE)GetProcAddress(hmOpenGL, "glBegin"), (LPBYTE)&Hooked_glBegin);
	pglColor4f = (glColor4f_t)DetourFunction((LPBYTE)GetProcAddress(hmOpenGL, "glColor4f"), (LPBYTE)&Hooked_glColor4f);
	pglVertex3fv = (glVertex3fv_t)DetourFunction((LPBYTE)GetProcAddress(hmOpenGL, "glVertex3fv"), (LPBYTE)&Hooked_glVertex3fv);
	pglClear = (glClear_t)DetourFunction((LPBYTE)GetProcAddress(hmOpenGL, "glClear"), (LPBYTE)&Hooked_glClear);
	pwglSwapBuffers = (wglSwapBuffers_t)DetourFunction((LPBYTE)GetProcAddress(hmOpenGL, "wglSwapBuffers"), (LPBYTE)&Hooked_wglSwapBuffers);
	pglViewport = (glViewport_t)DetourFunction((LPBYTE)GetProcAddress(hmOpenGL, "glViewport"), (LPBYTE)&Hooked_glViewport);
}

#pragma optimize("", on)  