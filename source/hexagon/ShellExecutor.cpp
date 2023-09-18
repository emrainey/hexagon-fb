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

#include "hexagon/Hexagon.h"

ShellExecutor::ShellExecutor() {
    // do nothing
}

ShellExecutor::~ShellExecutor() {
    // do nothing
}

bool ShellExecutor::bangCommand(std::string command) {
    if (command[0] == '!') {
        debug->info(Debug::Subsystem::Internal, "bangCommand: %s\n", command.c_str());

        // it's a bang command
        if (command == BANG_COMMAND_TEXT_TOGGLE) {
            toggleDisplayText();
            return true;
        } else if (command == BANG_COMMAND_ADD_VIEW) {
            createNewView();
            return true;
        } else if (command == BANG_COMMAND_MOUSE_CTRL) {
            toggleControlState();
            return true;
        } else if (command == BANG_COMMAND_FULLSCREEN) {
            glutFullScreen();
            return true;
        }
    }
    return false;
}
