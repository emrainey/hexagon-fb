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

#ifndef POSIX_FS_H
#define POSIX_FS_H

#include "hexagon/FileSystem.h"

// The Posix wrapper for file system functionality.
class PosixFileSystem : public FileSystem {
public:
#define REAL_SYSTEM_DELIMITER ('\\')
#define SYSTEM_DELIMITER ('/')
#define ROOT_PATH ("/")

    // class constructor
    PosixFileSystem();
    // class destructor
    ~PosixFileSystem() = default;

    std::string translateToExternal(std::string path) override;
    std::string translateToInternal(std::string path) override;
    std::list<std::string> *getDirectoryList(std::string path) override;
    std::string getMetaData(std::string path, std::string name) override;
    void populateNodeList(Layer *layer, std::string path) override;
    Node_Type_e getNodeType(std::string path, std::string name) override;
    Model_Type_e getModelType(std::string path, std::string name) override;
    char const *setCurrentDirectory(std::string directory) override;
};

#endif  // POSIX_FS_H
