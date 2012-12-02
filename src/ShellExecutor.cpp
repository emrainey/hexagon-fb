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
 
#include "Hexagon.h"

ShellExecutor::ShellExecutor()
{
    // do nothing
}

ShellExecutor::~ShellExecutor()
{
    // do nothing
}

bool ShellExecutor::bangCommand(string command)
{
    if (command[0] == '!')
    {
        debug->info(DEBUG_INTERNAL,"bangCommand: %s\n",command.c_str());
    
        // it's a bang command
        if (command == BANG_COMMAND_TEXT_TOGGLE)
        {
            toggleDisplayText();
        }
        else if (command == BANG_COMMAND_ADD_VIEW)
        {
            createNewView();
        }
        else if (command == BANG_COMMAND_MOUSE_CTRL)
        {
            toggleControlState();
        }
        else if (command == BANG_COMMAND_FULLSCREEN)
        {
            glutFullScreen();
        }
    }
}

