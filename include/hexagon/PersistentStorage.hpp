/**
 * COPYRIGHT (C) 2003-2026, ERIK RAINEY.
 *
 * This source code has not yet been approved for release under any license
 * and as such has no warranty or fitness of purpose associated with it. Use at
 * your own discretion.
 *
 * @author Erik Rainey
 * @file PersistentStorage.hpp
 */

#ifndef PERSISTENTSTORAGE_H
#define PERSISTENTSTORAGE_H

#include <string>

#include "hexagon/Key.hpp"

namespace hexagon {

/**
 * This object represents the basic attributes that a persistent database of
 * information about the project has. If a particular port wants to use a
 * different mechanism than another, then great. Everyone calls through the
 * object interface and everyone is happy.
 */
class PersistentStorage {
public:
    /**
     * Adds the supplied parameter to PersistentStorage. If there is
     * a collision then the existing Key is overwritten. No return code
     * is given.
     */
    virtual void setKey(Key k) = 0;
    /**
     * This method fetches a Key object out of the PersistentStorage.
     */
    virtual Key *getKey(std::string name) = 0;
};


} // namespace hexagon

#endif    // PERSISTENTSTORAGE_H
