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

#ifndef POSIX_EXEC_H
#define POSIX_EXEC_H

#include <unistd.h>

#include "hexagon/ShellExecutor.h"

/**
 * Posix Implementation of the ShellExecutor
 */
class PosixExec : public ShellExecutor {
public:
    // class constructor
    PosixExec();
    // class destructor
    ~PosixExec();

    /**
     * Initializes the Executor object.
     * @param command The command to execute.
     * @param path The full path in internal format!
     * @param file The file to execute on.
     */
    void initialize(std::string command, std::string path, std::string file);
    /**
     * Executes the command on the file.
     */
    bool execute();
};

#endif  // POSIX_EXEC_H
