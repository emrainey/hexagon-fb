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

#ifndef LINEARMENUOPTION_H
#define LINEARMENUOPTION_H

#include <string>

#include "hexagon/Debug.h"
#include "hexagon/MenuOption.h"
#include "hexagon/ShellExecutor.h"

/**
 * A linearly laid-out menu option.
 */
class LinearMenuOption : public MenuOption {
private:
    /** The displayed name of the option */
    std::string name;

    /** The shortcut letter */
    int letter;

    /** The pointer to the execution object */
    ShellExecutor *shell;

    /** The bounds of the menu options rectangle (for choose). */
    int x1, y1, x2, y2;

    /** The rendering inset */
    int inset;

    /** The linear index of this option */
    int index;

    /** The height of the MenuOption */
    int item_height;

    /** Calculates the outer bounds of this menu item */
    void calcBounds();

public:
    // class constructor
    LinearMenuOption();
    // class destructor
    ~LinearMenuOption();

    /**
     * Determines is a point was selected in the rectangle of the option.
     * @param index The index of the option in the menu, zero based.
     * @param x The x value of the selection point.
     * @param y The y value of the selection point.
     */
    bool choose(int index, int x, int y);

    /**
     * Renders the menu option
     * @param index The index of the option in the menu, zero based.
     * @param ix The x value of the render point.
     * @param iy The y value of the render point.
     */
    void render(int index, int ix, int iy);

    /**
     * Sets the MenuOptions Width.
     * @param width The new width.
     */
    void setWidth(int width);

    /**
     * Sets the MenuOption's Inset which is the distance from one
     * menu item to it's border.
     * @param i The inset to use around all items.
     */
    void setInset(int i);

    /**
     * @param n Sets the name of the option.
     */
    void setName(std::string n);

    /**
     * @return Returns the name of the MenuOption
     */
    std::string getName();

    /**
     * Sets the ShellExecutor Object to call once the item is called.
     * @param se A pointer to a shell executor object, initialized to the
     *           correct parameters.
     * @param parameters The parameters to pass to the executor.
     */
    void setShellExecutor(ShellExecutor *sh);

    /**
     * Calls the ShellExecutor object to execute the command.
     * @param parameters The parameters to pass to the executor.
     */
    void execute(std::string parameters);

    /**
     * Sets the index of this MenuOption in the menu, zero based.
     * @param index Zero based index.
     */
    void setIndex(int index);
};

#endif  // LINEARMENUOPTION_H
