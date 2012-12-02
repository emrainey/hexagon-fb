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

#ifndef WIN32FS_H
#define WIN32FS_H

// The Win32 wrapper for file system functionality.
class Win32FS : public FileSystem
{
	public:
        #define REAL_SYSTEM_DELIMITER   ('\\')
        #define SYSTEM_DELIMITER        ('/')
        #define ROOT_PATH               ("/")

        // class constructor
		Win32FS();
        // class destructor
        ~Win32FS();
		
		/*
		 * This function translates the path to the system represnetation 
		 * from the internal represenation.
		 */
		string translateToExternal(string path);
		
		/*
		 * This function translates a string from the system representation
		 * to the internal representation.
		 */
		string translateToInternal(string path);
		
		// This function returns, as strings, the contents of this directory.
		list<string> * getDirectoryList(string path);
		
		// This returns a string description of the item in question.
		string *getMetaData(string path, string name);
		
		/*
		 * This function takes all the files and directories or whatever 
		 * of a directory and turns them into a node list, complete with 
		 * initialized Nodes.
		 */
		void populateNodeList(Layer *layer, string path);
		
		// Gets the Node Type of the Node.
		Node_Type_e getNodeType(string path, string name);
		
        // Gets the Model Type of the Node
        Model_Type_e getModelType(string path, string name);
        
		// Sets the current directory...
		char * setCurrentDirectory(string directory);
};

#endif // WIN32FS_H

