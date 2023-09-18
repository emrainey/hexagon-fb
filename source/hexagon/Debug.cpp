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

Debug::Debug() : state{false}, subsystems{} {
    info(Debug::Subsystem::Trace, "Debug(%s)\n", (state ? "true" : "false"));
}

// class constructor
Debug::Debug(bool s) : state{s}, subsystems{} {
    info(Debug::Subsystem::Trace, "Debug(%s)\n", (state ? "true" : "false"));
}

// class destructor
Debug::~Debug() { info(Debug::Subsystem::Trace, "~Debug()\n"); }

void Debug::info(Subsystem subsystem, char const* const format, ...) {
    if (state == true) {
        if (not subsystems.IsSet(subsystem)) {
            return;
        }

        int len;
        va_list args;
        static char buffer[1024];

        va_start(args, format);
        vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);

        len = strlen(buffer);
        if (len > 0) {
            if (buffer[len - 1] == '\n') {
                buffer[len - 1] = '\0';
            }
        }
        printf("[0x%08x & 0x%08x] %s", subsystems.Get(), subsystems.AsMask(subsystem), buffer);
    }
}

// Wrapper to most flush operations
void Debug::flush(void) { fflush(stdout); }

void Debug::enable(Subsystem subsystem) {
    subsystems.Set(subsystem);
    info(Debug::Subsystem::Info, "Enabling Subsystem");
}

// Disables a susbystem from printing debug. See #defines.
void Debug::disable(Subsystem subsystem) {
    subsystems.Clear(subsystem);
    info(Debug::Subsystem::Info, "Disabling Subsystem");
}
