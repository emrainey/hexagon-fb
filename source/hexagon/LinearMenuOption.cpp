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

#include "hexagon/Hexagon.h"

// class constructor
LinearMenuOption::LinearMenuOption() {
    // nothing so far...
}

MenuOption::~MenuOption() { ; }

// class destructor
LinearMenuOption::~LinearMenuOption() { shell = nullptr; }

/**
 * Determines is a point was selected in the rectangle of the option.
 * @param index The index of the option in the menu, zero based.
 * @param x The x value of the selection point.
 * @param y The y value of the selection point.
 */
bool LinearMenuOption::choose(int index, int offset_x, int offset_y) {
    calcBounds();

    debug.info(Debug::Subsystem::Menu, "[%i] Selection @ {%i,%i}, Rect={%i,%i}->{%i,%i}\n", index, offset_x, offset_y,
               x1, y1, x2, y2);

    if ((offset_x > x1 && offset_x < x2) && (offset_y > y1 && offset_y < y2)) {
        execute("");
        return true;
    }

    return false;
}

/**
 * Renders the menu option
 * @param index The index of the option in the menu, zero based.
 * @param ix The x value of the render point.
 * @param iy The y value of the render point.
 */
void LinearMenuOption::render(int index, int ix, int iy) {
    calcBounds();

    // CAUTION! x1,y1;x2,y2 are unadjusted relative coordinates, they can't
    // be used to calculate the actually x,y, only the widths and what not.

    // draw the text...
    glColor3f(1.0, 1.0, 1.0);
    platform->display.print2DText(ix + x1 + inset, iy + y1 + item_height - (2 * inset), "%s", name.c_str());

    // draw the border...
    glBegin(GL_LINES);
    // top line
    glVertex2f(ix + x1, iy + y1);
    glVertex2f(ix + x2, iy + y1);
    // bottom line
    glVertex2f(ix + x1, iy + y2);
    glVertex2f(ix + x2, iy + y2);
    // left line
    glVertex2f(ix + x1, iy + y1);
    glVertex2f(ix + x1, iy + y2);
    // right lines
    glVertex2f(ix + x2 - 1, iy + y2);
    glVertex2f(ix + x2 - 1, iy + y2);
    glEnd();

    // draw the backdrop...
    glColor4f(0.7, 0.1, 0.7, 0.3);
    glRecti(ix + x1, iy + y1, ix + x2, iy + y2);
}

/**
 * Sets the MenuOptions Width.
 * @param width The new width.
 */
void LinearMenuOption::setWidth(int width) {
    debug.info(Debug::Subsystem::Trace, "LinearMenuOption::setWidth(%i)\n", width);
    x2 = width;
}

/*
 * Sets the LinearMenuOption's Inset which is the distance from one
 * menu item to it's border.
 */
void LinearMenuOption::setInset(int i) { inset = i; }

/**
 * Returns the name of the LinearMenuOption
 */
std::string LinearMenuOption::getName() { return name; }

void LinearMenuOption::setName(std::string n) { name = n; }

void LinearMenuOption::setShellExecutor(ShellExecutor *sh) { shell = sh; }

void LinearMenuOption::execute(std::string parameters) {
    if (shell != NULL) {
        shell->execute();
    }
}

void LinearMenuOption::setIndex(int i) { index = i; }

void LinearMenuOption::calcBounds() {
    // based on index and font height and inset, calculate x1,y1 and x2,y2
    // these points are relative to the zero of the menu.
    item_height = platform->display.font_height + (2 * inset);
    x1 = 0;
    y1 = index * item_height;
    // x2 is set by setWidth();
    y2 = (index + 1) * item_height;
}
