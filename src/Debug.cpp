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
#define CASE_DEBUG(x)       case x: printf("["#x"] "); break
#define DEBUG_DEFAULT       (DEBUG_TRACE | DEBUG_MENU | DEBUG_SELECTION)

Debug::Debug()
{
    state = false;
    systems = DEBUG_DEFAULT;
    info(DEBUG_TRACE,"Debug(%s)\n",(state?"true":"false"));
}

// class constructor
Debug::Debug(bool s)
{
    state = s;
    systems = DEBUG_DEFAULT;
    info(DEBUG_TRACE,"Debug(%s)\n",(state?"true":"false"));
}

// class destructor
Debug::~Debug()
{
    info(DEBUG_TRACE,"~Debug()\n");
}

void Debug::info(int subsystem, char *format,...)
{
    if (state == true)
    {
        //printf("[0x%08x & 0x%08x]\n",systems,subsystem);
    
        if ((systems & subsystem) == 0)
            return;
    
        int len;
        va_list args;
        static char buffer[1024];

        va_start(args, format);
        vsprintf(buffer, format, args);
        va_end(args);

        len = strlen(buffer);
        type(subsystem);
        cout << buffer;
    }
}
    
// Wrapper to most flush operations
void Debug::flush(void)
{
	fflush(stdout);
}

void Debug::type(int type)
{
    switch(type)
    {
        CASE_DEBUG(DEBUG_NONE);
        CASE_DEBUG(DEBUG_SELECTION);
        CASE_DEBUG(DEBUG_MENU);
        CASE_DEBUG(DEBUG_MODELS);
        CASE_DEBUG(DEBUG_INTERNAL);
        CASE_DEBUG(DEBUG_PLATFORM);
        CASE_DEBUG(DEBUG_TRACE);
        CASE_DEBUG(DEBUG_ANY);
        default:
            printf("[%d] ");
    }
}

/*
 * Enables a subsystem for debugging. See bit field #defines.
 */
void Debug::enableSubSystem(int subsystem)
{
    systems |= subsystem;
    info(DEBUG_ANY,"Enabling Subsystem");
}

// Disables a susbystem from printing debug. See #defines.
void Debug::disableSubSystem(int subsystem)
{
	systems &= ~subsystem;
	info(DEBUG_ANY,"Disabling Subsystem");
}

