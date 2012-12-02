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

#ifndef KEY_H
#define KEY_H

/**
 * This is the object to use with persistent storage. Anything that 
 * can be stored is stored as a key. The format of the storage is left
 * to the port implementation.
 */
class Key
{
	private:
        string name;
        string value;
	public:
		// class constructor
		Key(string n, string v);
		// class destructor
		~Key();
		// gets the value of the key
        string getValue();
        // gets the name of the key
        string getName();
        // sets the value of the key
        void setValue(string v);
        //sets the name of the key
        void setName(string n);
};

#endif // KEY_H

