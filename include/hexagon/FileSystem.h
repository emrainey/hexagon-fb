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

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <list>
#include <string>

#include "hexagon/Layer.h"
#include "hexagon/Node.h"

/**
 * The Interface for file system functionality.
 */
class FileSystem {
public:
    /**
     * This function translates the path to the system representation
     * from the internal representation.
     * @param path The platform neutral path to convert.
     * @return Returns the
     */
    virtual std::string translateToExternal(std::string path) = 0;

    /**
     * This function translates a std::string from the system representation
     * to the internal representation.
     */
    virtual std::string translateToInternal(std::string path) = 0;

    /**
     * This function returns, as std::strings, the contents of this directory.
     */
    virtual std::list<std::string> *getDirectoryList(std::string path) = 0;

    /**
     * This returns a std::string description of the item in question.
     * @param path The absolute path to the object in system neutral format.
     * @param name The name of the object to retriever information about.
     */
    virtual std::string getMetaData(std::string path, std::string name) = 0;

    /**
     * This function takes all the files and directories or whatever
     * of a directory and turns them into a node list, complete with
     * initialized Nodes.
     */
    virtual void populateNodeList(Layer *layer, std::string path) = 0;

    /// Gets the Node Type of the Node.
    virtual Node_Type_e getNodeType(std::string path, std::string name) = 0;

    /// Gets the Model Type of the Node
    virtual Model_Type_e getModelType(std::string path, std::string name) = 0;

    /// Sets the current directory...
    virtual char const *setCurrentDirectory(std::string directory) = 0;
};

#endif  // FILESYSTEM_H
