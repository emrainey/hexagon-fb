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

#ifndef MENU_H
#define MENU_H

#include "hexagon/MenuOption.h"

typedef void (*Void_Function_t)(void);

class Menu {
public:
    // the list of menu options
    std::list<MenuOption *> menu_options;

    // the initial selection offset
    int num_options;
    int next_y;
    int max_width;
    int max_height;
    int last_render_x;
    int last_render_y;

#define MENU_WIDTH (100)

    // class constructor
    Menu(std::string name);
    // class destructor
    ~Menu();
    // No description
    void addOption(std::string name, int letter, ShellExecutor *shell);
    // No description
    void render(int x, int y);
    // Finds the option selected.
    bool choose(int x, int y);
};

#endif  // MENU_H
