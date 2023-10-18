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

#include "hexagon/Hexagon.h"  // class's header file

// class constructor
Win32Exec::Win32Exec() {
    // insert your code here
}

// class destructor
Win32Exec::~Win32Exec() {
    // insert your code here
}

void Win32Exec::initialize(std::string c, std::string p, std::string f) {
    command = c;
    path = p;
    file = f;
}

bool Win32Exec::execute() {
    if (bangCommand(command) == true) return true;

    path = platform->fs->translateToExternal(path);

    if (debug.state) {
        std::string fullpath = path + REAL_SYSTEM_DELIMITER + file;
        debug.info(Debug::Subsystem::Platform, "Attempting to %s %s\n", command.c_str(), fullpath.c_str());
    }

    HINSTANCE rc = ShellExecute(NULL, command.c_str(), file.c_str(), NULL, path.c_str(), SW_SHOWNORMAL);

    if (rc <= (HINSTANCE)32) {
        switch ((int)rc) {
            case 0:
                debug.info(Debug::Subsystem::Info, "The system is out of resources.\n");
                break;
            case SE_ERR_NOASSOC:
            case SE_ERR_ASSOCINCOMPLETE:
                debug.info(Debug::Subsystem::Info, "There is no action associated with that file type.\n");
                break;
            case SE_ERR_ACCESSDENIED:
                debug.info(Debug::Subsystem::Info, "Access denied.\n");
                break;
            default:
                debug.info(Debug::Subsystem::Info, "Unknown error code (%i) returned from ShellExecute().\n", rc);
                break;
        }
    } else if (debug.state) {
        debug.info(Debug::Subsystem::Platform, "Please wait while \"%s\" is %s in \"%s\".\n", file.c_str(),
                    command.c_str(), path.c_str());
    }
}
