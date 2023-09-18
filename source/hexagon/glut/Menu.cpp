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

#define INSET (4)
#define FONT_WIDTH (10)

// class constructor
Menu::Menu(std::string name) {
    debug->info(Debug::Subsystem::Trace, "+Menu()\n");
    next_y = 0;
    num_options = 0;
    max_width = 0;
    max_height = platform->display.font_height;
    debug->info(Debug::Subsystem::Menu, "Menu::max_height = %i\n", max_height);
    addOption(name, 0, NULL);
    debug->info(Debug::Subsystem::Trace, "-Menu()\n");
}

// class destructor
Menu::~Menu() {
    std::list<MenuOption *>::iterator lit = menu_options.begin();
    while (lit != menu_options.end()) {
        delete (*lit);
        lit++;
    }
}

// No description
void Menu::addOption(std::string name, int letter, ShellExecutor *shell) {
    debug->info(Debug::Subsystem::Trace, "+Menu::addOption(%s)\n", name.c_str());

    int width = name.size() * FONT_WIDTH;
    debug->info(Debug::Subsystem::Menu, "new width=%i, max_width=%i\n", width, max_width);
    if (width > max_width) {
        max_width = width;

        // reset everyone's width
        for (auto menu_opt : menu_options) {
            menu_opt->setWidth(max_width);
        }
    }

    MenuOption *m = new LinearMenuOption();
    m->setInset(INSET);
    m->setName(name);
    m->setShellExecutor(shell);
    m->setIndex(num_options);
    m->setWidth(max_width);
    menu_options.push_back(m);
    num_options++;

    debug->info(Debug::Subsystem::Trace, "-Menu::addOption()\n");
}

// No description
void Menu::render(int ix, int iy) {
    int index = 0;

    // will we need to alter the ix,iy if it goes off screen?
    if (ix + max_width > platform->display.width) {
        ix -= (ix + max_width) - platform->display.width;
    }

    int item_height = max_height + (2 * INSET);
    int menu_height = item_height * num_options;

    if (iy + menu_height > platform->display.height) {
        iy -= (iy + menu_height) - platform->display.height;
    }

    // save the initial positions (this will save the position a couple times).
    last_render_x = ix;
    last_render_y = iy;

    std::list<MenuOption *>::iterator lit = menu_options.begin();
    while (lit != menu_options.end()) {
        (*lit)->render(index++, ix, iy);
        lit++;
    }
}

// Finds the option selected.
bool Menu::choose(int x, int y) {
    debug->info(Debug::Subsystem::Menu, "Selected @ {%i,%i}, Last @ {%i,%i};\n", x, y, last_render_x, last_render_y);

    int index = 0;
    int offset_x = x - last_render_x;
    int offset_y = y - last_render_y;

    std::list<MenuOption *>::iterator lit = menu_options.begin();
    while (lit != menu_options.end()) {
        if ((*lit)->choose(index++, offset_x, offset_y) == true) {
            debug->info(Debug::Subsystem::Menu, "Option %s selected!\n", (*lit)->getName().c_str());
            break;
        }
        lit++;
    }
}
