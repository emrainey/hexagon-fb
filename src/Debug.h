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

#ifndef DEBUG_H
#define DEBUG_H

#define DEBUG_NONE          (0x00000000)    // turns all off
#define DEBUG_SELECTION     (0x00000001)    // this deals with the Z Buffer Hit
#define DEBUG_MENU          (0x00000002)    // this deals with platforms menu
#define DEBUG_MODELS        (0x00000004)    // this deals with loading issues
#define DEBUG_INTERNAL      (0x00000008)    // this describes internal issues
#define DEBUG_PLATFORM      (0x00000010)    // this deals with platform issues
#define DEBUG_TRACE         (0x00000020)    // function tracing...
#define DEBUG_ANY           (0xFFFFFFFF)    // if any debug is on this will go.

// Super Class of all debugging classes.
class Debug
{
	public:
		// Is debug on or off?
		bool state;
		
		// which subsystems are enabled?
		unsigned int systems;
		
  		// class constructor
        Debug();
		Debug(bool state);
		
		// class destructor
		~Debug();
		
        // typical info popup call 
		void info(int subsystem, char *format,...);
		
        // Wrapper to most flush operations
		void flush(void);
		
		/**
         * Disables a susbystem from printing debug. See #defines.
         */
		void disableSubSystem(int subsystem);
		
		/**
		 * Enables a subsystem for debugging. See bit field #defines.
		 */
		void enableSubSystem(int subsystem);
        
    protected:
        void type(int type);

};

#endif // DEBUG_H

