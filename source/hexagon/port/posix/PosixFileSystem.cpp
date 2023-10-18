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

#include <filesystem>
#include <iostream>
#include <list>
#include <set>
#include <string>

namespace fs = std::filesystem;

#include "hexagon/port/posix/PosixFileSystem.h"

PosixFileSystem::PosixFileSystem() {}

std::string PosixFileSystem::translateToInternal(std::string path) { return path; }

std::string PosixFileSystem::translateToExternal(std::string path) { return path; }

void PosixFileSystem::populateNodeList(Layer *layer, std::string path) {
    // we need to get the list of files in this directory...
    std::list<std::string> *list_of_names = getDirectoryList(path);
    for (auto name : *list_of_names) {
        layer->addNode(path, name);
    }
    delete list_of_names;
}

// This function returns, as std::strings, the contents of this directory.
std::list<std::string> *PosixFileSystem::getDirectoryList(std::string path) {
    std::list<std::string> *list_of_names = new std::list<std::string>();
    setCurrentDirectory(path);

    for (const auto &entry : fs::directory_iterator(path)) {
        list_of_names->push_back(entry.path().filename().string());
    }
    return list_of_names;
}

// This returns a std::string description of the item in question.
std::string PosixFileSystem::getMetaData(std::string _path, std::string name) {
    fs::path path = _path;
    fs::path fullpath = path / name;
    if (fs::is_directory(fullpath)) {
        return std::string("Directory");
    } else if (fs::is_regular_file(fullpath)) {
        return std::string("File");
    } else if (fs::is_symlink(fullpath)) {
        return std::string("Symbolic Link");
    } else {
        return std::string("Unknown");
    }
}

// Gets the Node Type of the file system item
Node_Type_e PosixFileSystem::getNodeType(std::string path, std::string name) { return Node_Type_e::DEFAULT; }

Model_Type_e PosixFileSystem::getModelType(std::string path, std::string name) { return Model_Type_e::FOLDER; }

// Sets the current directory...
char const *PosixFileSystem::setCurrentDirectory(std::string directory) {
    fs::path _dir = directory;
    std::filesystem::current_path(_dir);
    return directory.c_str();
}
