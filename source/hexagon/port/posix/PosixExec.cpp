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

#include "hexagon/port/posix/PosixExec.h"  // class's header file

#include "hexagon/Debug.h"
#include "hexagon/port/Port.h"

// class constructor
PosixExec::PosixExec() {
    // insert your code here
}

// class destructor
PosixExec::~PosixExec() {
    // insert your code here
}

void PosixExec::initialize(std::string c, std::string p, std::string f) {
    command = c;
    path = p;
    file = f;
}

bool PosixExec::execute() {
    if (bangCommand(command)) {
        return true;
    }

    path = platform->fs->translateToExternal(path);

    if (debug.state) {
        std::string fullpath = path + REAL_SYSTEM_DELIMITER + file;
        debug.info(Debug::Subsystem::Platform, "Attempting to %s %s\n", command.c_str(), fullpath.c_str());
    }

    // FIXME exec the command
    return false;
}
