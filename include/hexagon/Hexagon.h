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

// #warning Including Hexagon

// include the standard stuff...
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <iostream>

// include some new stuff....
// #include <stl.h>
#include <list>
#include <stack>
#include <string>
#include <type_traits>

/*****************************************************************************/
// Some MACROS
/*****************************************************************************/

#define PI (3.141592653)
#define TWO_PI (6.283185306)
#define SIN60 (0.866025403784438646763723170752936)
#define COS30 (SIN60)
#define SQRT3 (1.73205080756887729352744634150587)
#define FPS (30)

#define VERSION ("v1.0.10 alpha")
#define BUILD_STRING ("in C++")

/* These macros convert cos and sin to degree inputs not redians. */

constexpr double Rad2Deg(double rad) { return (rad / TWO_PI) * 360.0; }
constexpr double Deg2Rad(double deg) { return (deg / 360.0) * TWO_PI; }
#define COS(n) cos(Deg2Rad(n))
#define SIN(n) sin(Deg2Rad(n))

constexpr void toggle(bool &a) { a = not a; }
constexpr bool toogle(bool a) { return not a; }

/*****************************************************************************/
// Include all the Object Definitions
/*****************************************************************************/

// base classes and simpe objects
#include "hexagon/Choreographer.h"
#include "hexagon/Color.h"
#include "hexagon/Debug.h"
#include "hexagon/Iconcache.h"
#include "hexagon/Key.h"
#include "hexagon/Light.h"
#include "hexagon/Model.h"
#include "hexagon/Vector.h"

// containers for Not so simple objects
#include "hexagon/Layer.h"
#include "hexagon/Node.h"
#include "hexagon/View.h"

// Generic Interfaces
#include "hexagon/ActionRetriever.h"
#include "hexagon/FileSystem.h"
#include "hexagon/MenuOption.h"
#include "hexagon/PersistentStorage.h"
#include "hexagon/ShellExecutor.h"
#include "hexagon/World.h"
#include "hexagon/glut/Menu.h"

#ifndef _MENU_TYPE_
#define _MENU_TYPE_
#include "hexagon/LinearMenuOption.h"
#endif

#include "hexagon/port/Port.h"

/*****************************************************************************/
// Now for the globals
/*****************************************************************************/

#ifndef EXCLUDE_EXTERNS
extern Port *platform;
extern World *world;
#endif

/*****************************************************************************/
#endif
/*****************************************************************************/
