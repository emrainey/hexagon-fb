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

#include <time.h>
#include "Hexagon.h"

// define a pointer the Port object
extern Port *platform;

// class constructor
Scheduler::Scheduler()
{
	// insert your code here
	debug->info(DEBUG_TRACE,"Scheduler()\n");
}

// class destructor
Scheduler::~Scheduler()
{
	// insert your code here
}

// GLUT Callback for the Timed Scheduler
void Scheduler::timed(int value)
{
    static int counter = 0;

    if (platform == NULL)
        return;

    //debug->info(DEBUG_TRACE,"+Scheduler::timer()\n");

	// if we are in the new selected state, then set the selected object's
	// render call item index to selected.
	
    // else if context menu was selected throw up the thing for that

    // move the camera
    platform->camera.move();
	
	// move all the objects
    world->focus->top->move();
    
    // come up with new color to blend to
    if ((++counter) % 20 == 0)
    {
        //platform->display.bg.perturb(rand());
    }
    
    //debug->info(DEBUG_TRACE,"-Scheduler::timer()\n");
}

// GLUT Default Schedulerr Callback
void Scheduler::idle(void)
{
    if (platform == NULL)
        return;

	static clock_t diff;

    diff = timer;
    timer = clock();
    diff = timer - diff;
    //debug->info("Idle last called %i usecs ago\n",diff);

    /* give up the time slice */
    Sleep(1);

	glutPostRedisplay();
}

