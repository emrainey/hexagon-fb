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

#define DIRECTORY_CHANGE_RETRY (3)
#define LD_MAX (108)
#define FS_NAME_MAX (128)
#define VOL_NAME_MAX (128)
#define REALLY_LONG_DESCRIPTION (1024)
#define BIG_BUFFER (4096)

// class constructor
PosixFileSystem::PosixFileSystem() {
    // insert your code here
}

// class destructor
PosixFileSystem::~PosixFileSystem() {
    // insert your code here
}

/*
 * This function translates a std::string from the system representation
 * to the internal representation.
 */
string PosixFileSystem::translateToInternal(std::string path) {
    for (int i = 0; i < path.length(); i++)
        if (path[i] == REAL_SYSTEM_DELIMITER) path[i] = SYSTEM_DELIMITER;
    return path;
}

/*
 * This function translates the path to the system representation
 * from the internal represenation.
 */
string PosixFileSystem::translateToExternal(std::string path) {
    path.erase(0, 1);
    for (int i = 0; i < path.length(); i++)
        if (path[i] == SYSTEM_DELIMITER) path[i] = REAL_SYSTEM_DELIMITER;
    return path;
}

/*
 * This function takes all the files and directories or whatever
 * of a directory and turns them into a node list, complete with
 * initialized Nodes.
 */
void PosixFileSystem::populateNodeList(
    Layer *layer, std::string path) { /* TODO (#1#): Implement PosixFileSystem::populateNodeList() */
}

// This function returns, as std::strings, the contents of this directory.
std::list<std::string> *PosixFileSystem::getDirectoryList(std::string path) {
    std::list<std::string> *bob = new std::list<std::string>();
    return bob;
}

// This returns a std::string description of the item in question.
string *PosixFileSystem::getMetaData(std::string path, std::string name) {
    if (path == ROOT_PATH) {
        char logicaldrives[LD_MAX];
        char volname[VOL_NAME_MAX];
        char fsname[FS_NAME_MAX];
        char description[REALLY_LONG_DESCRIPTION];
        int rc;
        uint32_t drivetype;
        unsigned long long int free_space = 0, size = 0, free_user = 0;
        uint32_t serial, filenamelen, fsflags;

        /* initialize structures */
        memset(fsname, 0, FS_NAME_MAX);
        memset(volname, 0, VOL_NAME_MAX);
        serial = 0;
        filenamelen = 0;

        GetVolumeInformation(name.c_str(), volname, VOL_NAME_MAX, (unsigned long *)&serial,
                             (unsigned long *)&filenamelen, (unsigned long *)&fsflags, fsname, FS_NAME_MAX);

        // The format for the extra information should be...
        /*
            VOLUME_NAME (FS_TYPE)
            Serial #
        */
        memset(description, 0, sizeof(description));
        sprintf(description, "%s (%s)\nSerial Number: %08x", volname, fsname, serial);

        return new std::string(description);
    } else {
        std::string *d = new std::string();
        std::string fullpath = path + SYSTEM_DELIMITER + name;
        fullpath = translateToExternal(fullpath);
        DWORD attributes = GetFileAttributes(fullpath.c_str());
        debug->info(Debug::Subsystem::Platform, "Attributes=0x%08x\n", attributes);
        if ((attributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) {
            *d += "Directory\n";
        } else {
            HANDLE hdl = CreateFile(fullpath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
                                    FILE_ATTRIBUTE_NORMAL, NULL);
            DWORD size = 0;
            DWORD hisize = 0;
            // compute the size of the file...
            size = GetFileSize(hdl, &hisize);
            CloseHandle(hdl);

            char filesize[255];
            memset(filesize, 0, sizeof(filesize));
            sprintf(filesize, "%u %s\n", (size < 1024 ? size : (size / 1024)), (size < 1024 ? "bytes" : "kb"));

            *d += filesize;
        }
        *d += "[";
        if ((attributes & FILE_ATTRIBUTE_READONLY) == FILE_ATTRIBUTE_READONLY) *d += "R";
        if ((attributes & FILE_ATTRIBUTE_HIDDEN) == FILE_ATTRIBUTE_HIDDEN) *d += "H";
        if ((attributes & FILE_ATTRIBUTE_SYSTEM) == FILE_ATTRIBUTE_SYSTEM) *d += "S";
        if ((attributes & FILE_ATTRIBUTE_ARCHIVE) == FILE_ATTRIBUTE_ARCHIVE) *d += "A";
        if ((attributes & FILE_ATTRIBUTE_COMPRESSED) == FILE_ATTRIBUTE_COMPRESSED) *d += "C";
        if ((attributes & FILE_ATTRIBUTE_ENCRYPTED) == FILE_ATTRIBUTE_ENCRYPTED) *d += "E";
        *d += "]\n";
        return d;
    }
}

// Gets the Node Type of the file system item
Node_Type_e PosixFileSystem::getNodeType(std::string path, std::string name) {
    debug->info(Debug::Subsystem::Platform, "---------------------------------------------------\n");
    if (path == ROOT_PATH) {
        debug->info(Debug::Subsystem::Platform, "%s is a root directory\n", name.c_str());
        return DIRECTORY_TYPE;
    } else {
        std::string fullpath = path + SYSTEM_DELIMITER + name;
        fullpath = translateToExternal(fullpath);
        debug->info(Debug::Subsystem::Platform, "FullPath=%s\n", fullpath.c_str());
        DWORD attributes = GetFileAttributes(fullpath.c_str());
        debug->info(Debug::Subsystem::Platform, "Attribute: 0x%08x\n", attributes);
        debug->info(Debug::Subsystem::Platform, "FILE_ATTRIBUTE_DIRECTORY=0x%08x\n", FILE_ATTRIBUTE_DIRECTORY);
        if ((attributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) {
            debug->info(Debug::Subsystem::Platform, "%s is a directory\n", name.c_str());
            return DIRECTORY_TYPE;
        } else  // if (attribute & FILE_ATTRIBUTE_NORMAL >
        {
            debug->info(Debug::Subsystem::Platform, "%s is a file\n", name.c_str());
            return FILE_ON_DISK_TYPE;
        }
    }
}

Model_Type_e PosixFileSystem::getModelType(std::string path, std::string name) {
    if (path == ROOT_PATH) {
        uint32_t drivetype = GetDriveType(name.c_str());
        switch (drivetype) {
            case DRIVE_REMOVABLE:
                return REMOVABLE_DRIVE;

            case DRIVE_FIXED:
                return FIXED_DRIVE;

            case DRIVE_REMOTE:
                return REMOTE_DRIVE;

            case DRIVE_CDROM:
                return CDROM_DRIVE;

            case DRIVE_RAMDISK:
                return RAMDISK_DRIVE;

            case 1:
                // debug->info("Drive %s does not exist.\n",name.c_str());
            case 0:
                /****************************/
                /* intentional fall-through */
                /****************************/
            default:
                return FOLDER;
        }
    } else {
        std::string fullpath = path + SYSTEM_DELIMITER + name;
        fullpath = translateToExternal(fullpath);
        DWORD attributes = GetFileAttributes(fullpath.c_str());
        if (attributes & FILE_ATTRIBUTE_DIRECTORY > 0)
            return FOLDER;
        else
            return USER_DEFINED;
    }
}

// Sets the current directory...
char *PosixFileSystem::setCurrentDirectory(std::string directory) {
    bool done = false;
    int retry_count = 0;
    while (!done) {
        // they want a normal directory listing
        // "temp","file1.txt","Program Files","file2.txt"

        // first we have to "set" our current directory
        directory = translateToExternal(directory);
        debug->info(Debug::Subsystem::Platform, "Setting Current Directory to %s\n", directory.c_str());
        SetCurrentDirectory(directory.c_str());
        char fullpath[255];
        GetCurrentDirectory(255, fullpath);
        debug->info(Debug::Subsystem::Platform, "Current Directory is %s\n", fullpath);

        // trim the slash off the return std::string
        int len = strlen(fullpath);
        if (fullpath[len - 1] == REAL_SYSTEM_DELIMITER) fullpath[len - 1] = '\0';

        // did we change directories at all?
        if (strcmp(fullpath, directory.c_str()) != 0) {
            debug->info(Debug::Subsystem::Platform, "Couldn't change directories to that path! Trying again!\n");

            retry_count++;
            if (retry_count > DIRECTORY_CHANGE_RETRY) return NULL;

            // change the path to the root of that drive...
            char simple[3];
            sprintf(simple, "%c:\\", directory[0]);
            SetCurrentDirectory(simple);
        } else {
            done = true;
        }
    }
}
