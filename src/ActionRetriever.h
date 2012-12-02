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

#ifndef ACTIONRETRIEVER_H
#define ACTIONRETRIEVER_H

/**
 * An ActionRetriever implements the ability to retrieve file specific action
 * menus for any file system item, including Hexagon specific items. The
 * implementor of this class will also make certain that the specifics of the
 * callbacks are taken care of as well. If the requested item is a non-file,
 * that is a directory, then use the constants defined here in order to
 * retrieve it's menu.
 */
class ActionRetriever
{
    public:

        // file system specific stuff...
        #define ACTION_DIRECTORY ("!directory")
        #define ACTION_DRIVE     ("!drive")
        
        // Hexagon specific stuff
        #define ACTION_LAYER     ("!layer")
        #define ACTION_UP        ("!up")
        #define ACTION_VIEW      ("!view")
        #define ACTION_DEFAULT   ("!default")

        ActionRetriever();
        ~ActionRetriever();

        /**
         * This method returns Menu pointers for Hexagon specific menus
         * @param fileType A "banged" description on the item to retrieve a menu
         *                 for.
         */
        Menu *bangCommand(string fileType);

        /**
         * Gets all the actions associated with this particular file type.
         * @param fileType A string representing the fileType of the item to
         *                 retrieve information about. For example: "txt"
         *                 Note: Special Types are transmitted using the
         *                 directives found in this header.
         * NOTE: You must call bangCommand() as the first action of this method
         * to ensure proper platform-agnostic behavior.
         */
        virtual Menu *getActions(string fileType) = 0;
        
        /**
         * Gets all the actions associated with a directory for this platform.
         */
        virtual Menu *getDirectoryActions() = 0;
        
        /**
         * Gets all the actions associated with a drive for this platform.
         */
        virtual Menu *getDriveActions() = 0;
};

#endif // ACTIONRETRIEVER_H

