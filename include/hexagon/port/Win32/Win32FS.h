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

#ifndef WIN32FS_H
#define WIN32FS_H

// The Win32 wrapper for file system functionality.
class Win32FS : public FileSystem {
public:
#define REAL_SYSTEM_DELIMITER ('\\')
#define SYSTEM_DELIMITER ('/')
#define ROOT_PATH ("/")

    // class constructor
    Win32FS();
    // class destructor
    ~Win32FS();

    /*
     * This function translates the path to the system represnetation
     * from the internal represenation.
     */
    std::string translateToExternal(std::string path);

    /*
     * This function translates a std::string from the system representation
     * to the internal representation.
     */
    std::string translateToInternal(std::string path);

    // This function returns, as std::strings, the contents of this directory.
    std::list<std::string> *getDirectoryList(std::string path);

    // This returns a std::string description of the item in question.
    std::string *getMetaData(std::string path, std::string name);

    /*
     * This function takes all the files and directories or whatever
     * of a directory and turns them into a node list, complete with
     * initialized Nodes.
     */
    void populateNodeList(Layer *layer, std::string path);

    // Gets the Node Type of the Node.
    Node_Type_e getNodeType(std::string path, std::string name);

    // Gets the Model Type of the Node
    Model_Type_e getModelType(std::string path, std::string name);

    // Sets the current directory...
    char const *setCurrentDirectory(std::string directory);
};

#endif  // WIN32FS_H
