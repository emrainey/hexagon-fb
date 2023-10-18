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

#ifndef VIEW_H
#define VIEW_H

#include <string>

#include "hexagon/Layer.h"
#include "hexagon/Model.h"
#include "hexagon/Vector.h"

// No description
class View {
public:
    // our neighbors
    View *neighbors[6];

    // the View contains a stack of layers
    // the layer on the back() is the "top" of the stack
    std::list<Layer *> layer_list;
#define MAX_LAYERS (255)

    // the current top layer
    Layer *top;

    // this is the name of the layer
    std::string name;

    // the rendering position of the view
    Vector position;

    // a pointer to the model to use for the view
    Model *model;

#define VIEW_SPACE_MASK (0xFF000000)
    // the pre-fix for selection naming for this view.
    int name_space;

    // class constructor
    View(int select_name);
    View(int select_name, std::string name);
    void init(int select_name);

    // class destructor
    ~View();

    // Renders the view
    void render(bool selection, SelectionState mode);
    // Allows the selection engine to select something.
    bool choose(int name);
    // Adds a layer to the view
    void pushLayer(std::string path);
    // Removes the top level layer
    void popLayer(void);
};

#endif  // VIEW_H
