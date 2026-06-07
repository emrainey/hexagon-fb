/**
 * COPYRIGHT (C) 2003-2026, ERIK RAINEY.
 *
 * This source code has not yet been approved for release under any license
 * and as such has no warranty or fitness of purpose associated with it. Use at
 * your own discretion.
 *
 * @author Erik Rainey
 * @file Port.hpp
 */

#ifndef PORT_H
#define PORT_H

// #warning Included Port

#include <glut.h>

#include "hexagon/glut/Camera.hpp"
#include "hexagon/glut/Display.hpp"
#include "hexagon/glut/Input.hpp"
#include "hexagon/glut/Menu.hpp"
#include "hexagon/glut/Scheduler.hpp"

// port specific stuff...
#ifdef __WIN32__
#include <windows.h>
#include <winreg.h>
/// separate
#include <hexagon/port/Win32/Win32AR.hpp>
#include <hexagon/port/Win32/Win32Exec.hpp>
#include <hexagon/port/Win32/Win32Registry.hpp>

#define PORT_SPECIFIC_PERSISTENT_STORAGE Win32Registry
#define PORT_SPECIFIC_ACTION_RETRIEVER Win32AR
#define PORT_SPECIFIC_SHELL_EXECUTOR Win32Exec
#else
#include <hexagon/port/posix/PosixActionRetriever.hpp>
#include <hexagon/port/posix/PosixExec.hpp>
#include <hexagon/port/posix/PosixRegistry.hpp>

#define PORT_SPECIFIC_PERSISTENT_STORAGE PosixRegistry
#define PORT_SPECIFIC_ACTION_RETRIEVER PosixActionRetriever
#define PORT_SPECIFIC_SHELL_EXECUTOR PosixExec
#endif

typedef enum {

    MOUSE_DRAG,
    MOUSE_ZOOM,
    CONTROL_MOD,  // this must be the last value, or any state beyond will be
                  // ignored
    KEYBOARD_CONTROL
} Control_State_t;

/**
 * @class Port
 * @brief Platform porting and graphics context wrapper.
 */
class Port {
public:
    // GLUT related objects...
    Input input;
    Scheduler scheduler;
    Display display;  // this must be initialized before the camera
    Camera camera;
    Control_State_t control_state;

    // platform specific implementations of interfaces...
    PersistentStorage *ps;
    ActionRetriever *ar;
    ShellExecutor *shell;

    // class constructor
    Port(int argc, char *argv[]);
    // class destructor
    ~Port();

    // the function to get the party started!
    void run();
    /**
     * Do any post-instantiation initialization.
     */
    void postInitialize();
};

extern Port *platform;

#endif  // PORT_H
