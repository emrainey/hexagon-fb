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

#ifndef POSIX_AR_H
#define POSIX_AR_H

#include <string>

#include "hexagon/ActionRetriever.h"

/**
 * This class implements the ActionRetriever Object for Posix.
 */
class PosixActionRetriever : public ActionRetriever {
public:
    // class constructor
    PosixActionRetriever();
    // class destructor
    ~PosixActionRetriever();
    // see interface declaration
    Menu *getActions(std::string fileType);
    // see interface declaration
    Menu *getDirectoryActions();
    // see interface declaration
    Menu *getDriveActions();
};

#endif  // POSIX_AR_H
