/*******************************************************************************
 * COPYRIGHT (C) 2003-2026, ERIK RAINEY.
 *
 * This source code has not yet been approved for release under any license
 * and as such has no warranty or fitness of purpose associated with it. Use at
 * your own discretion.
 *
 * @author Erik Rainey
 * @file glut_emulation.hpp
 ******************************************************************************/

#ifndef GLUT_EMULATION_HPP
#define GLUT_EMULATION_HPP

struct GLFWwindow;

namespace hexagon {

/// GLUT constants
#define GLUT_RGBA            0x0000
#define GLUT_RGB             0x0000
#define GLUT_INDEX           0x0001
#define GLUT_SINGLE          0x0000
#define GLUT_DOUBLE          0x0002
#define GLUT_ACCUM           0x0004
#define GLUT_ALPHA           0x0008
#define GLUT_DEPTH           0x0010
#define GLUT_STENCIL         0x0020
#define GLUT_MULTISAMPLE     0x0080

/// Mouse buttons
#define GLUT_LEFT_BUTTON     0
#define GLUT_MIDDLE_BUTTON   1
#define GLUT_RIGHT_BUTTON    2

/// Mouse button state
#define GLUT_DOWN            0
#define GLUT_UP              1

/// Key modifiers
#define GLUT_ACTIVE_SHIFT    1
#define GLUT_ACTIVE_CTRL     2
#define GLUT_ACTIVE_ALT      4

/// Special keys
#define GLUT_KEY_F1          1
#define GLUT_KEY_F2          2
#define GLUT_KEY_F3          3
#define GLUT_KEY_F4          4
#define GLUT_KEY_F5          5
#define GLUT_KEY_F6          6
#define GLUT_KEY_F7          7
#define GLUT_KEY_F8          8
#define GLUT_KEY_F9          9
#define GLUT_KEY_UP          101
#define GLUT_KEY_DOWN        103
#define GLUT_KEY_PAGE_UP     104
#define GLUT_KEY_PAGE_DOWN   105

/// Cursor representation
#define GLUT_CURSOR_INHERIT  1

/// Bitmap Font representation
#define GLUT_BITMAP_9_BY_15  ((void*)1)

/// GLUT function declarations
void glutInit(int *argcp, char **argv);
void glutInitDisplayMode(unsigned int mode);
void glutInitWindowPosition(int x, int y);
void glutInitWindowSize(int width, int height);
int glutCreateWindow(const char *title);
void glutMainLoop(void);
void glutSwapBuffers(void);
void glutPostRedisplay(void);
void glutFullScreen(void);
void glutReshapeWindow(int width, int height);
void glutPositionWindow(int x, int y);
void glutSetCursor(int cursor);
void glutBitmapCharacter(void *font, int character);
int glutGetModifiers(void);

::GLFWwindow* glutGetGLFWWindow(void);

/// Callback registration
void glutMouseFunc(void (*func)(int button, int state, int x, int y));
void glutMotionFunc(void (*func)(int x, int y));
void glutPassiveMotionFunc(void (*func)(int x, int y));
void glutKeyboardFunc(void (*func)(unsigned char key, int x, int y));
void glutSpecialFunc(void (*func)(int key, int x, int y));
void glutReshapeFunc(void (*func)(int width, int height));
void glutDisplayFunc(void (*func)(void));
void glutTimerFunc(unsigned int millis, void (*func)(int value), int value);
void glutIdleFunc(void (*func)(void));


} // namespace hexagon

#endif // GLUT_EMULATION_HPP
