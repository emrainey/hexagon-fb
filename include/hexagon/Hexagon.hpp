/**
 * COPYRIGHT (C) 2003-2026, ERIK RAINEY.
 *
 * This source code has not yet been approved for release under any license
 * and as such has no warranty or fitness of purpose associated with it. Use at
 * your own discretion.
 *
 * @author Erik Rainey
 * @file Hexagon.hpp
 */

#ifndef _HEXAGON_H_
#define _HEXAGON_H_

// #warning Including Hexagon

// include the standard stuff...
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <thread>

#include <iostream>

// include some new stuff....
// #include <stl.h>
#include <list>
#include <numbers>
#include <stack>
#include <string>
#include <type_traits>

/*****************************************************************************/
// Some MACROS
/*****************************************************************************/

namespace std {
namespace numbers {
constexpr static auto twopi = std::numbers::pi * 2.0;
} // namespace numbers
} // namespace std

/* These macros convert cos and sin to degree inputs not redians. */

constexpr double Rad2Deg(double rad) {
    return (rad / std::numbers::twopi) * 360.0;
}
constexpr double Deg2Rad(double deg) {
    return (deg / 360.0) * std::numbers::twopi;
}

constexpr double COS(double n) { return cos(Deg2Rad(n)); }
constexpr double SIN(double n) { return sin(Deg2Rad(n)); }

constexpr void toggle(bool &a) {
    a = not a;
}
constexpr bool toogle(bool a) {
    return not a;
}

/*****************************************************************************/
// Include all the Object Definitions
/*****************************************************************************/

// base classes and simpe objects
#include "hexagon/Choreographer.hpp"
#include "hexagon/Color.hpp"
#include "hexagon/Debug.hpp"
#include "hexagon/Iconcache.hpp"
#include "hexagon/Key.hpp"
#include "hexagon/Light.hpp"
#include "hexagon/Model.hpp"
#include "hexagon/Vector.hpp"

// containers for Not so simple objects
#include "hexagon/Layer.hpp"
#include "hexagon/Node.hpp"
#include "hexagon/View.hpp"

// Generic Interfaces
#include "hexagon/ActionRetriever.hpp"
#include "hexagon/MenuOption.hpp"
#include "hexagon/PersistentStorage.hpp"
#include "hexagon/ShellExecutor.hpp"
#include "hexagon/World.hpp"
#include "hexagon/glut/Menu.hpp"

#ifndef _MENU_TYPE_
#define _MENU_TYPE_
#include "hexagon/LinearMenuOption.hpp"
#endif

#include "hexagon/port/Port.hpp"

namespace hexagon {

/*****************************************************************************/
// Now for the globals
/*****************************************************************************/

#ifndef EXCLUDE_EXTERNS
extern Port *platform;
extern World *world;
#endif

/*****************************************************************************/

} // namespace hexagon

#endif
/*****************************************************************************/
