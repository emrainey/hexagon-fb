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

#include "hexagon/Hexagon.h"  // class's header file

// class constructor
Win32AR::Win32AR() {
    // insert your code here
}

// class destructor
Win32AR::~Win32AR() {
    // insert your code here
}

Menu *Win32AR::getActions(std::string fileType) {
    Menu *m = bangCommand(fileType);
    if (m != NULL) return m;

    // get the file specific information...
    m = new Menu("File Menu");

    return m;
}

// see interface declaration
Menu *Win32AR::getDirectoryActions() {
    Menu *m = new Menu("Directory Menu");

    return m;
}

// see interface declaration
Menu *Win32AR::getDriveActions() {
    Menu *m = new Menu("Drive Menu");

    return m;
}
