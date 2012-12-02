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

#include "Hexagon.h" // class's header file

#define ROOT_KEY_LOCATION ("Software\\Toaster Streudel Productions\\Hexagon")

static HKEY root_key = NULL;

// class constructor
Win32Registry::Win32Registry()
{
    // do any initialization of the register that is neccessary!
	DWORD disposition = 0;
	PHKEY proot = &root_key;
        
    if (RegCreateKeyEx(HKEY_LOCAL_MACHINE,
                       ROOT_KEY_LOCATION,
                       0,
                       "String",
                       REG_OPTION_NON_VOLATILE,   // store it persistently!
                       KEY_ALL_ACCESS,            // R+W
                       NULL,                      // default Security
                       proot,
                       &disposition) == ERROR_SUCCESS)
    {
        // success!
        debug->info(DEBUG_PLATFORM,"Success in opening the Registry!\n");
        
        if (disposition == REG_CREATED_NEW_KEY)
        {
            debug->info(DEBUG_PLATFORM,"Root Key did not exist before!\n");
        }
        else if (disposition == REG_OPENED_EXISTING_KEY)
        {
            debug->info(DEBUG_PLATFORM,"Root Key existed before-hand.\n");
        }
    }
    else
    {
        debug->info(DEBUG_PLATFORM,"Failed to open registry!\n");
    }
}

// class destructor
Win32Registry::~Win32Registry()
{
	// do any tear-down needed here!
	RegCloseKey(root_key);
}

/**
 * This method fetches a Key object out of the PersistentStorage.
 */
Key *Win32Registry::getKey(string name)
{
    #define LOCAL_SIZE 255
    Key *k = NULL;
    char value[LOCAL_SIZE];
    DWORD size = LOCAL_SIZE;
    DWORD type = 0;
    
    debug->info(DEBUG_TRACE,"+getKey(\"%s\")\n",name.c_str());
    
    if (RegQueryValueEx(root_key,
                        name.c_str(),
                        0,
                        &type,
                        (BYTE *)value, 
                        &size) != ERROR_SUCCESS)
    {
        debug->info(DEBUG_PLATFORM,"RegQueryValueEx() failed!\n");
    }
    else
    {
        debug->info(DEBUG_PLATFORM,"RegQueryValueEx() Succeed!\n");
        debug->info(DEBUG_PLATFORM,"value==0x%08x\n",value);
        debug->info(DEBUG_PLATFORM,"size==%i\n",size);
        if (size != 0)
        {
            string v(value,strlen(value));
            k = new Key(name,v);
        }
    }
    
    debug->info(DEBUG_TRACE,"-getKey()\n");
    return k;
}

/**
 * Adds the supplied parameter to PersistentStorage. If there is
 * a collision then the existing Key is overwritten. No return code
 * is given.
 */
void Win32Registry::setKey(Key k)
{
    int len = strlen(k.getValue().c_str());
    if (RegSetValueEx(root_key,
                      k.getName().c_str(),
                      0,
                      REG_SZ,
                      (BYTE *)k.getValue().c_str(),
                      len) != ERROR_SUCCESS)
    {
        debug->info(DEBUG_PLATFORM,"RegSetValueEx() failed!\n");
    }
    else
    {
        debug->info(DEBUG_PLATFORM,"RegSetValueEx() Succeeded!\n");
    }
}

