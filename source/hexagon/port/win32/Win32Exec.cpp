/*******************************************************************************
 * COPYRIGHT (C) 2003-2026, ERIK RAINEY.
 *
 * This source code has not yet been approved for release under any license
 * and as such has no warranty or fitness of purpose associated with it. Use at
 * your own discretion.
 *
 * @author Erik Rainey
 ******************************************************************************/

#include <filesystem>

#include "hexagon/Hexagon.hpp"    // class's header file

namespace hexagon {

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

    std::filesystem::path p(path);
    p.make_preferred();
    path = p.string();

    if (debug.state) {
        std::filesystem::path fullpath = p / file;
        debug.info(Debug::Subsystem::Platform, "Attempting to %s %s\n", command.c_str(), fullpath.string().c_str());
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
        debug.info(Debug::Subsystem::Platform, "Please wait while \"%s\" is %s in \"%s\".\n", file.c_str(), command.c_str(), path.c_str());
    }
}

} // namespace hexagon
