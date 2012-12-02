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

#define EXCLUDE_EXTERNS // don't pull in the externs for platform and world.
#include "Hexagon.h"

// The pointer to all the port specific functions and variables
Port *platform = NULL;

// The pointer to all renderable content
World *world = NULL;

// The pointer the debug object
Debug *debug = NULL;

// set the default number of views
static int num_views = 1;

// set the default view location
static string *start_view = new string(ROOT_PATH);

void info(void)
{
    debug->info(DEBUG_ANY,"Hexagon, by Erik Rainey\n");
    debug->info(DEBUG_ANY,"Copyright 2002 (C) All Rights Reserved (for now...)\n");
}

void ProcessArgs(int argc, char *argv[])
{
    for (int i=1; i < argc; i++)
    {
        if (argv[i][0] == '/' || argv[i][0] == '-')
        {
            switch(argv[i][1])
            {
                case 'v':
                case 'V':
                    // verify argv[i+1] exists...
                    if (i+1 >= argc)
                        break;
                    // now read it...
                    num_views = atoi(argv[i+1]);
                    break;
                case 'o':
                case 'O':
                    // verify i+1 exists...
                    if (i+1 >= argc)
                        break;
                    // set the default directory to it...
                    start_view = new string(argv[i+1]);
                    debug->info(DEBUG_ANY,"Start Directory set to %s\n",start_view->c_str());
                    break;                        
                default:
                    break;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    // instantiate a new Debug object
    debug = new Debug();
    debug->state = true;

    info();

    // take the args and process them...
    ProcessArgs(argc,argv); 

    // instantiate a new port interface
    platform = new Port(argc, argv);
    platform->postInitialize();
    platform->display.initOpenGL();

    // <song> A whole new world!...</song>
    world = new World(num_views,start_view);

    // kick it off!
    platform->run();
    
    return 0;
}
