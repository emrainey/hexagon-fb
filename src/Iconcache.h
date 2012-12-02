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

#ifndef ICONCACHE_H
#define ICONCACHE_H

/*
 * The Icon Cache is the first object to ask for a new, unknown icon 
 * when a new file extension is found.
 */
class IconCache
{
	public:
		// class constructor
		IconCache();
		// class destructor
		~IconCache();
};

#endif // ICONCACHE_H

