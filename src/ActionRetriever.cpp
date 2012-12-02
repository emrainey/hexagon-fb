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

ActionRetriever::ActionRetriever()
{
    // do nothing
}

ActionRetriever::~ActionRetriever()
{
    // do nothing
}

Menu *ActionRetriever::bangCommand(string fileType)
{
    if (fileType[0] == '!')
    {
        debug->info(DEBUG_INTERNAL,"bangCommand: %s\n",fileType.c_str());

        // it's a bang command
        if (fileType == ACTION_DIRECTORY)
        {
            return getDirectoryActions();
        }
        else if (fileType == ACTION_DRIVE)
        {
            return getDriveActions();
        }
        else if (fileType == ACTION_LAYER)
        {
            Menu *m = new Menu("Layer Menu");

            return m;
        }
        else if (fileType == ACTION_UP)
        {
            Menu *m = new Menu("Up Menu");

            return m;
        }
        else if (fileType == ACTION_VIEW)
        {
            Menu *m = new Menu("View Menu");
            
            return m;
        }
        else if (fileType == ACTION_DEFAULT)
        {
            Menu *m = new Menu("Default Menu");

            ShellExecutor *s1 = new PORT_SPECIFIC_SHELL_EXECUTOR();
            s1->initialize(BANG_COMMAND_TEXT_TOGGLE,"","");

            ShellExecutor *s2 = new PORT_SPECIFIC_SHELL_EXECUTOR();
            s2->initialize(BANG_COMMAND_MOUSE_CTRL,"","");

            ShellExecutor *s3 = new PORT_SPECIFIC_SHELL_EXECUTOR();
            s3->initialize(BANG_COMMAND_ADD_VIEW,"","");

            ShellExecutor *s4 = new PORT_SPECIFIC_SHELL_EXECUTOR();
            s4->initialize(BANG_COMMAND_FULLSCREEN,"","");

            m->addOption("Text On/Off",0,s1);
            m->addOption("Mouse Freelook/Zoom",0,s2);
            m->addOption("Add View",0,s3);
            m->addOption("Full Screen",0,s4);
            return m;
        }
    }
}
