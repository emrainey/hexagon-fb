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

#include "hexagon/Debug.hpp"
#include "hexagon/port/Port.hpp"
#include "hexagon/port/posix/PosixExec.hpp"    // class's header file

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

    if (debug.state) {
        std::filesystem::path fullpath = std::filesystem::path(path) / file;
        debug.info(Debug::Subsystem::Platform, "Attempting to %s %s\n", command.c_str(), fullpath.string().c_str());
    }

    // FIXME exec the command
    return false;
}
