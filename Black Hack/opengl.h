typedef void (APIENTRY *glBegin_t)(GLenum);
typedef void (APIENTRY *glVertex3fv_t)(const GLfloat *v);
typedef void (APIENTRY *glColor4f_t)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef BOOL(APIENTRY *wglSwapBuffers_t)(HDC  hdc);
typedef void (APIENTRY *glClear_t)(GLbitfield mask);
typedef void (APIENTRY *glViewport_t)(GLint x, GLint y, GLsizei width, GLsizei height);

void HookOpenGL();