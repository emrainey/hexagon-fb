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

#include "hexagon/Hexagon.h"  // class's header file

// class constructor
Key::Key(std::string n, std::string v) {
    name = n;
    value = v;
}

// class destructor
Key::~Key() {
    // insert your code here
}

std::string Key::getName() { return name; }

std::string Key::getValue() { return value; }

void Key::setName(std::string n) { name = n; }

void Key::setValue(std::string v) { value = v; }
