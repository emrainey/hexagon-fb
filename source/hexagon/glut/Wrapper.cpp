/*******************************************************************************
 * COPYRIGHT (C) 2003-2026, ERIK RAINEY.
 *
 * This source code has not yet been approved for release under any license
 * and as such has no warranty or fitness of purpose associated with it. Use at
 * your own discretion.
 *
 * @author Erik Rainey
 ******************************************************************************/

#define EXCLUDE_EXTERNS
#include "hexagon/glut/Wrapper.hpp"

#include "hexagon/Debug.hpp"
#include "hexagon/port/Port.hpp"
#include "hexagon/glut/font8x8_basic.h"
#include <chrono>

extern Port *platform;
extern Debug debug;

// Global state for GLFW
static GLFWwindow *g_window = nullptr;
static int g_windowWidth = 640;
static int g_windowHeight = 480;
static int g_windowX = 100;
static int g_windowY = 100;

// Callback function pointers
static void (*g_mouseClickFunc)(int button, int state, int x, int y) = nullptr;
static void (*g_mouseDragFunc)(int x, int y) = nullptr;
static void (*g_mouseMoveFunc)(int x, int y) = nullptr;
static void (*g_keyPressedFunc)(unsigned char key, int x, int y) = nullptr;
static void (*g_keyPressedSpecialFunc)(int key, int x, int y) = nullptr;
static void (*g_reshapeScreenFunc)(int width, int height) = nullptr;
static void (*g_drawScreenFunc)(void) = nullptr;
static void (*g_idleLoopFunc)(void) = nullptr;
static void (*g_mouseWheelFunc)(int wheel, int direction, int x, int y) = nullptr;

// Timer state
struct Timer {
    std::chrono::steady_clock::time_point fire_time;
    void (*func)(int);
    int value;
    bool active;
};
static Timer g_timer = { std::chrono::steady_clock::now(), nullptr, 0, false };

void glutTimerFunc(unsigned int millis, void (*func)(int value), int value) {
    g_timer.fire_time = std::chrono::steady_clock::now() + std::chrono::milliseconds(millis);
    g_timer.func = func;
    g_timer.value = value;
    g_timer.active = true;
}

// GLFW Callbacks
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action != GLFW_PRESS && action != GLFW_REPEAT) return;

    int special_key = -1;
    switch (key) {
        case GLFW_KEY_F1: special_key = GLUT_KEY_F1; break;
        case GLFW_KEY_F2: special_key = GLUT_KEY_F2; break;
        case GLFW_KEY_F3: special_key = GLUT_KEY_F3; break;
        case GLFW_KEY_F4: special_key = GLUT_KEY_F4; break;
        case GLFW_KEY_F5: special_key = GLUT_KEY_F5; break;
        case GLFW_KEY_F6: special_key = GLUT_KEY_F6; break;
        case GLFW_KEY_F7: special_key = GLUT_KEY_F7; break;
        case GLFW_KEY_F8: special_key = GLUT_KEY_F8; break;
        case GLFW_KEY_F9: special_key = GLUT_KEY_F9; break;
        case GLFW_KEY_UP: special_key = GLUT_KEY_UP; break;
        case GLFW_KEY_DOWN: special_key = GLUT_KEY_DOWN; break;
        case GLFW_KEY_PAGE_UP: special_key = GLUT_KEY_PAGE_UP; break;
        case GLFW_KEY_PAGE_DOWN: special_key = GLUT_KEY_PAGE_DOWN; break;
    }

    if (special_key != -1) {
        if (g_keyPressedSpecialFunc) {
            g_keyPressedSpecialFunc(special_key, 0, 0);
        }
    } else {
        if (key == GLFW_KEY_ESCAPE) {
            if (g_keyPressedFunc) g_keyPressedFunc(27, 0, 0);
        } else if (key == GLFW_KEY_ENTER || key == GLFW_KEY_KP_ENTER) {
            if (g_keyPressedFunc) g_keyPressedFunc(13, 0, 0);
        }
    }
}

static void char_callback(GLFWwindow* window, unsigned int codepoint) {
    if (codepoint < 128) {
        if (g_keyPressedFunc) {
            g_keyPressedFunc((unsigned char)codepoint, 0, 0);
        }
    }
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    int glut_button = -1;
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        glut_button = GLUT_LEFT_BUTTON;
    } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        glut_button = GLUT_RIGHT_BUTTON;
    }

    int glut_state = (action == GLFW_PRESS) ? GLUT_DOWN : GLUT_UP;

    if (glut_button != -1 && g_mouseClickFunc) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        g_mouseClickFunc(glut_button, glut_state, (int)xpos, (int)ypos);
    }
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    bool pressed = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS ||
                    glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);
    if (pressed) {
        if (g_mouseDragFunc) {
            g_mouseDragFunc((int)xpos, (int)ypos);
        }
    } else {
        if (g_mouseMoveFunc) {
            g_mouseMoveFunc((int)xpos, (int)ypos);
        }
    }
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    if (g_mouseWheelFunc) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        int direction = (yoffset > 0.0) ? 1 : -1;
        g_mouseWheelFunc(0, direction, (int)xpos, (int)ypos);
    }
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    if (g_reshapeScreenFunc) {
        g_reshapeScreenFunc(width, height);
    }
}

// Custom GLUT Emulations
void glutInit(int *argcp, char **argv) {
    if (!glfwInit()) {
        debug.info(Debug::Subsystem::Error, "Failed to initialize GLFW\n");
        exit(1);
    }
}

void glutInitDisplayMode(unsigned int mode) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    if (mode & GLUT_MULTISAMPLE) {
        glfwWindowHint(GLFW_SAMPLES, 4);
    }
}

void glutInitWindowPosition(int x, int y) {
    g_windowX = x;
    g_windowY = y;
}

void glutInitWindowSize(int width, int height) {
    g_windowWidth = width;
    g_windowHeight = height;
}

int glutCreateWindow(const char *title) {
    g_window = glfwCreateWindow(g_windowWidth, g_windowHeight, title, NULL, NULL);
    if (!g_window) {
        debug.info(Debug::Subsystem::Error, "Failed to create GLFW window\n");
        glfwTerminate();
        exit(1);
    }

    glfwSetWindowPos(g_window, g_windowX, g_windowY);
    glfwMakeContextCurrent(g_window);

    glfwSetKeyCallback(g_window, key_callback);
    glfwSetCharCallback(g_window, char_callback);
    glfwSetMouseButtonCallback(g_window, mouse_button_callback);
    glfwSetCursorPosCallback(g_window, cursor_position_callback);
    glfwSetScrollCallback(g_window, scroll_callback);
    glfwSetFramebufferSizeCallback(g_window, framebuffer_size_callback);

    if (platform) {
        platform->window = g_window;
    }

    return 1;
}

void glutSwapBuffers(void) {
    if (g_window) {
        glfwSwapBuffers(g_window);
    }
}

void glutPostRedisplay(void) {
    // no-op (main loop draws continuously)
}

void glutFullScreen(void) {
    if (g_window) {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(g_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    }
}

void glutReshapeWindow(int width, int height) {
    if (g_window) {
        glfwSetWindowMonitor(g_window, NULL, 50, 50, width, height, 0);
    }
}

void glutPositionWindow(int x, int y) {
    if (g_window) {
        glfwSetWindowPos(g_window, x, y);
    }
}

void glutSetCursor(int cursor) {
    // no-op
}

void glutBitmapCharacter(void *font, int character) {
    if (character < 0 || character >= 128) {
        return;
    }
    GLubyte flipped[8];
    for (int i = 0; i < 8; i++) {
        flipped[i] = font8x8_basic[character][7 - i];
    }
    GLint align;
    GLboolean lsb_first;
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &align);
    glGetBooleanv(GL_UNPACK_LSB_FIRST, &lsb_first);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_LSB_FIRST, GL_FALSE);

    glBitmap(8, 8, 0.0f, 0.0f, 9.0f, 0.0f, flipped);

    glPixelStorei(GL_UNPACK_ALIGNMENT, align);
    glPixelStorei(GL_UNPACK_LSB_FIRST, lsb_first);
}

GLFWwindow* glutGetGLFWWindow(void) {
    return g_window;
}

int glutGetModifiers(void) {
    int modifiers = 0;
    if (g_window) {
        if (glfwGetKey(g_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ||
            glfwGetKey(g_window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) {
            modifiers |= GLUT_ACTIVE_SHIFT;
        }
        if (glfwGetKey(g_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ||
            glfwGetKey(g_window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS) {
            modifiers |= GLUT_ACTIVE_CTRL;
        }
        if (glfwGetKey(g_window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS ||
            glfwGetKey(g_window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS) {
            modifiers |= GLUT_ACTIVE_ALT;
        }
    }
    return modifiers;
}

void glutMainLoop(void) {
    while (!glfwWindowShouldClose(g_window)) {
        glfwPollEvents();

        if (g_timer.active) {
            auto now = std::chrono::steady_clock::now();
            if (now >= g_timer.fire_time) {
                g_timer.active = false;
                if (g_timer.func) {
                    g_timer.func(g_timer.value);
                }
            }
        }

        if (g_idleLoopFunc) {
            g_idleLoopFunc();
        }

        if (g_drawScreenFunc) {
            g_drawScreenFunc();
        }
    }
}

void glutMouseFunc(void (*func)(int button, int state, int x, int y)) { g_mouseClickFunc = func; }
void glutMotionFunc(void (*func)(int x, int y)) { g_mouseDragFunc = func; }
void glutPassiveMotionFunc(void (*func)(int x, int y)) { g_mouseMoveFunc = func; }
void glutKeyboardFunc(void (*func)(unsigned char key, int x, int y)) { g_keyPressedFunc = func; }
void glutSpecialFunc(void (*func)(int key, int x, int y)) { g_keyPressedSpecialFunc = func; }
void glutReshapeFunc(void (*func)(int width, int height)) { g_reshapeScreenFunc = func; }
void glutDisplayFunc(void (*func)(void)) { g_drawScreenFunc = func; }
void glutIdleFunc(void (*func)(void)) { g_idleLoopFunc = func; }
void glutMouseWheelFunc(void (*func)(int wheel, int direction, int x, int y)) { g_mouseWheelFunc = func; }

#define TEST_PLATFORM()                                                    \
    {                                                                      \
        if (platform == NULL) {                                            \
            debug.info(Debug::Subsystem::Internal, "Platform is NULL!\n"); \
            return;                                                        \
            }                                                              \
    }

// The thin GLUT wrappers. These are needed because we can't cast these Class
// Functions into the necessary GLUT function types.
void glut_keyPressed(unsigned char key, int x, int y) {
    TEST_PLATFORM();
    platform->input.keyboardNormal(key, x, y);
}

void glut_keyPressedSpecial(int key, int x, int y) {
    TEST_PLATFORM();
    platform->input.keyboardSpecial(key, x, y);
}

void glut_mouseClick(int button, int state, int x, int y) {
    TEST_PLATFORM();
    platform->input.mouseClick(button, state, x, y);
}

void glut_mouseWheel(int wheel, int direction, int x, int y) {
    TEST_PLATFORM();
    platform->input.mouseWheel(wheel, direction, x, y);
}

void glut_mouseDrag(int x, int y) {
    TEST_PLATFORM();
    platform->input.mouseDrag(x, y);
}

void glut_mouseMove(int x, int y) {
    TEST_PLATFORM();
    platform->input.mouseMove(x, y);
}

void glut_reshapeScreen(int width, int height) {
    TEST_PLATFORM();
    platform->display.resize(width, height);
}

void glut_drawScreen(void) {
    TEST_PLATFORM();
    platform->display.draw();
}

void glut_timerFired(int value) {
    if (platform != NULL) {
        platform->scheduler.timed(value);
    }

    /* reinstall the timer */
    glutTimerFunc(5, glut_timerFired, 0);
}

void glut_idleLoop(void) {
    TEST_PLATFORM();
    platform->scheduler.idle();
}

// This function just does all the assignment for the wrappers
void glutWrappers_assignCallbacks(void) {
    // Input Object Callbacks
    glutMouseFunc(glut_mouseClick);
    glutMotionFunc(glut_mouseDrag);
    glutMouseWheelFunc(glut_mouseWheel);
    glutPassiveMotionFunc(glut_mouseMove);
    glutKeyboardFunc(glut_keyPressed);
    glutSpecialFunc(glut_keyPressedSpecial);

    // Display Object Callbacks
    glutReshapeFunc(glut_reshapeScreen);
    glutDisplayFunc(glut_drawScreen);

    // Timer and Idle Callbacks
    glutTimerFunc(5, glut_timerFired, 0);
    glutIdleFunc(glut_idleLoop);
}

void glLocalPushMatrix(char const *const filename, int const line) {
    glPushMatrix();
    CheckError(glGetError(), true, "Local Push", filename, line);
}

void glLocalPopMatrix(char const *const filename, int const line) {
    glPopMatrix();
    CheckError(glGetError(), true, "Local Pop", filename, line);
}

GLuint CheckError(GLuint error, bool stop, char const *const name, char const *const filename, int const line) {
    switch (error) {
        case GL_NO_ERROR:
            // don't bother checking anything else.
            return error;
        case GL_INVALID_ENUM:
            debug.info(Debug::Subsystem::Error, "ERROR: Invalid enumeration %s=0x%08x in %s on line %i\n", name, error, filename, line);
            break;
        case GL_INVALID_VALUE:
            debug.info(Debug::Subsystem::Error, "ERROR: Invalid value %s=0x%08x in %s on line %i\n", name, error, filename, line);
            break;
        case GL_INVALID_OPERATION:
            debug.info(Debug::Subsystem::Error, "ERROR: Invalid operation %s=0x%08x in %s on line %i\n", name, error, filename, line);
            break;
        case GL_STACK_OVERFLOW:
            debug.info(Debug::Subsystem::Error, "ERROR: Stack overflow @%s in %s on line %i!\n", name, filename, line);
            break;
        case GL_STACK_UNDERFLOW:
            debug.info(Debug::Subsystem::Error, "ERROR: Stack underflow @%s in %s on line %i!\n", name, filename, line);
            break;
        case GL_OUT_OF_MEMORY:
            debug.info(Debug::Subsystem::Error, "ERROR: Out Of Memory @%s in %s on line %i!\n", name, filename, line);
            break;
        default:
            debug.info(Debug::Subsystem::Error, "ERROR: Unknown Error %d (0x%08x) @%s in %s on line %i!\n", error, error, name, filename, line);
            break;
    }
    if (stop == true) {
        /* add any deconstructors here */
        exit(1);
    }

    return error;
}

void report_error(char const *const name, bool stop, char const *const filename, int const line) {
    GLuint _error;
    do {
        _error = glGetError();
        CheckError(_error, stop, name, filename, line);
    } while (_error != GL_NO_ERROR);
}

/*****************************************************************************/
/* END                                                                       */
/*****************************************************************************/
