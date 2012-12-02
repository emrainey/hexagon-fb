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

#ifndef WIN32EXEC_H
#define WIN32EXEC_H

/**
 * Win32 Implementation of the ShellExecutor
 */
class Win32Exec : public ShellExecutor
{
    public:
        // class constructor
        Win32Exec();
        // class destructor
        ~Win32Exec();

        /**
         * Initializes the Executor object.
         * @param command The command to execute.
         * @param path The full path in internal format!
         * @param file The file to execute on.
         */
        void initialize(string command, string path, string file);
        /**
         * Executes the command on the file.
         */
        bool execute();
};

#endif // WIN32EXEC_H

