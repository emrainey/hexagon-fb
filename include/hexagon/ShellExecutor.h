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

#ifndef SHELLEXECUTOR_H
#define SHELLEXECUTOR_H

#include <list>
#include <string>

#define BANG_COMMAND_TEXT_TOGGLE ("!text")
#define BANG_COMMAND_ADD_VIEW ("!view")
#define BANG_COMMAND_MOUSE_CTRL ("!mousectrl")
#define BANG_COMMAND_FULLSCREEN ("!fullscreen")

/**
 * A Shell Executor takes commands to pass to the operating system in a
 * system neutral fashion.
 */
class ShellExecutor {
protected:
    std::string command;
    std::string path;
    std::string file;

public:
    ShellExecutor();
    ~ShellExecutor();
    bool bangCommand(std::string command);
    /**
     * Initializes the Executor object.
     * @param command The command to execute.
     * @param path The full path in internal format!
     * @param file The file to execute on.
     */
    virtual void initialize(std::string command, std::string path, std::string file) = 0;
    /**
     * Executes the command on the file.
     * NOTE: You must call bangCommand() as the first action of this method.
     */
    virtual bool execute() = 0;
};

#endif  // SHELLEXECUTOR_H
