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

#ifndef WIN32AR_H
#define WIN32AR_H

/**
 * This class implements the ActionRetriever Object for Win32.
 */
class Win32AR : public ActionRetriever
{
	public:
		// class constructor
		Win32AR();
		// class destructor
		~Win32AR();
		// see interface declaration
		Menu *getActions(string fileType);
        // see interface declaration
        Menu *getDirectoryActions();
        // see interface declaration
        Menu *getDriveActions();
};

#endif // WIN32AR_H

