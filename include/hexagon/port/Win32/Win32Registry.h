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

#ifndef WIN32REGISTRY_H
#define WIN32REGISTRY_H

/**
 * This object represents the basic attributes that a persistent database of
 * information about the project has. If a particular port wants to use a
 * different mechanism than another, then great. Everyone calls through the
 * object interface and everyone is happy.
 */
class Win32Registry : public PersistentStorage {
public:
    // class constructor
    Win32Registry();
    // class destructor
    ~Win32Registry();
    /**
     * Adds the supplied parameter to PersistentStorage. If there is
     * a collision then the existing Key is overwritten. No return code
     * is given.
     */
    void setKey(Key k);
    /**
     * This method fetches a Key object out of the PersistentStorage.
     */
    Key *getKey(std::string name);
};

#endif  // WIN32REGISTRY_H
