/*******************************************************************************
 * COPYRIGHT (C) 2003, ERIK RAINEY.
 *
 * This source code has not yet been approved for release under any license
 * and as such has no warranty or fitness of purpose associated with it. Use at
 * your own discretion.
 *
 * @author Erik Rainey
 * @version 1.0
 ******************************************************************************/

#ifndef PORT_H
#define PORT_H

// #warning Included Port

#include <glut.h>

#include "hexagon/glut/Camera.h"
#include "hexagon/glut/Display.h"
#include "hexagon/glut/Input.h"
#include "hexagon/glut/Menu.h"
#include "hexagon/glut/Scheduler.h"

// port specific stuff...
#ifdef __WIN32__
#include <windows.h>
#include <winreg.h>
/// separate
#include <hexagon/port/Win32/Win32AR.h>
#include <hexagon/port/Win32/Win32Exec.h>
#include <hexagon/port/Win32/Win32FS.h>
#include <hexagon/port/Win32/Win32Registry.h>

#define PORT_SPECIFIC_PERSISTENT_STORAGE Win32Registry
#define PORT_SPECIFIC_FILE_SYSTEM Win32FS
#define PORT_SPECIFIC_ACTION_RETRIEVER Win32AR
#define PORT_SPECIFIC_SHELL_EXECUTOR Win32Exec
#else
#include <hexagon/port/posix/PosixActionRetriever.h>
#include <hexagon/port/posix/PosixExec.h>
#include <hexagon/port/posix/PosixFileSystem.h>
#include <hexagon/port/posix/PosixRegistry.h>

#define PORT_SPECIFIC_PERSISTENT_STORAGE PosixRegistry
#define PORT_SPECIFIC_FILE_SYSTEM PosixFileSystem
#define PORT_SPECIFIC_ACTION_RETRIEVER Posix2ActionRetriever
#define PORT_SPECIFIC_SHELL_EXECUTOR PosixExec
#endif

typedef enum {

    MOUSE_DRAG,
    MOUSE_ZOOM,
    CONTROL_MOD,  // this must be the last value, or any state beyond will be
                  // ignored
    KEYBOARD_CONTROL
} Control_State_t;

// No description
class Port {
public:
    // GLUT related objects...
    Input input;
    Scheduler scheduler;
    Display display;  // this must be initialized before the camera
    Camera camera;
    Control_State_t control_state;

    // platform specific implementations of interfaces...
    FileSystem *fs;
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

#endif  // PORT_H
