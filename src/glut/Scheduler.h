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

#ifndef SCHEDULER_H
#define SCHEDULER_H

// No description
class Scheduler
{
	public:
	    clock_t timer;
	    clock_t start;
	    clock_t finish;
	    
		// class constructor
		Scheduler();
		// class destructor
		~Scheduler();
		// GLUT Default Idler Callback
		void idle(void);
		// GLUT Callback for the Timer
		void timed(int value);
};

#endif // IDLE_H

