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

#ifndef _HEXAGON_H_
#define _HEXAGON_H_

//#warning Including Hexagon

// include the standard stuff...
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// include some new stuff....
//#include <stl.h>
#include <list>
#include <stack>
#include <string>

using namespace std;

/*****************************************************************************/
// Some MACROS
/*****************************************************************************/

#define PI                      (3.141592653)
#define TWO_PI                  (6.283185306)
#define SIN60					(0.866025403784438646763723170752936)
#define COS30					(SIN60)
#define SQRT3					(1.73205080756887729352744634150587)
#define FPS                     (30)

#define VERSION                 ("v1.0.10 alpha")
#define BUILD_STRING            ("in C++")

/*****************************************************************************/
// Some FUNCTION MACROS
/*****************************************************************************/

#define GL_PUSH_MATRIX()    glLocalPushMatrix(__FILE__,__LINE__)
#define GL_POP_MATRIX()     glLocalPopMatrix(__FILE__,__LINE__)

/* simply toggles the ls bit */
#define TOGGLE_BOOL(a) {        \
    if (a == false)             \
        a = true;               \
    else                        \
        a = false;              \
}

#define REPORT_ERROR(name,stop)  {                          \
    GLuint error = glGetError();                            \
    do {                                                    \
        CheckError(error,stop,#name,__FILE__,__LINE__);     \
    } while (error != GL_NO_ERROR);                         \
}
    
#define CHECKERROR(a,b)	CheckError(a,b,#a,__FILE__,__LINE__)

/* These macros convert cos and sin to degree inputs not redians. */

#define Rad2Deg(rad)    (((double)(rad)/TWO_PI)*360)
#define Deg2Rad(deg)    (((double)(deg)/360)*TWO_PI)
#define COS(n)          cos(Deg2Rad(n))
#define SIN(n)          sin(Deg2Rad(n))

/*****************************************************************************/
// Some Typedefs
/*****************************************************************************/

// we need to get rid of these...
typedef unsigned int GLenum;
typedef unsigned int GLuint;
typedef float GLfloat;

/*****************************************************************************/
// Some prototypes
/*****************************************************************************/
void glLocalPushMatrix(char *filename, int line);
void glLocalPopMatrix(char *filename, int line);
void glutWrappers_assignCallbacks(void);
GLuint CheckError(GLuint error, bool stop, char *name, char *filename, int line);
// No description
void toggleDisplayText(void);
void toggleControlState(void);
void createNewView(void);

/*****************************************************************************/
// Include all the Object Definitions
/*****************************************************************************/

// base classes and simpe objects
#include "Debug.h"
#include "Vector.h"
#include "Model.h"
#include "Light.h"
#include "Choreographer.h"
#include "Key.h"            
#include "IconCache.h"
#include "Color.h"

// containers for Not so simple objects
#include "Node.h"
#include "Layer.h"
#include "View.h"

// Generic Interfaces
#include "PersistentStorage.h"
#include "FileSystem.h"
#include "ShellExecutor.h"
#include "MenuOption.h"
#include "Menu.h"
#include "ActionRetriever.h"

#include "World.h"

#ifndef _MENU_TYPE_
#define _MENU_TYPE_
#include "LinearMenuOption.h"
#endif

#include <Port.h>

/*****************************************************************************/
// Now for the globals 
/*****************************************************************************/

#ifndef EXCLUDE_EXTERNS 
extern Port *platform;
extern World *world;
extern Debug *debug;
#endif

/*****************************************************************************/
#endif
/*****************************************************************************/
