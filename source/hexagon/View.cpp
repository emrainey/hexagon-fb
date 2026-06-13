/*******************************************************************************
 * COPYRIGHT (C) 2003-2026, ERIK RAINEY.
 *
 * This source code has not yet been approved for release under any license
 * and as such has no warranty or fitness of purpose associated with it. Use at
 * your own discretion.
 *
 * @author Erik Rainey
 ******************************************************************************/

#include "hexagon/Hexagon.hpp"

namespace hexagon {

// class constructor
View::View(int select_name) {
    name = "Default View";
    init(select_name);
}

View::View(int select_name, std::string n) {
    name = n;
    init(select_name);
}

// class destructor
View::~View() {
    // insert your code here
}

void View::init(int select_name) {
    // create a model for ourselves
    model = new Model("");
    model->select_name = select_name;
    name_space = select_name;

    // clear out our neighbors graph
    memset(neighbors, 0, sizeof(neighbors));

    // initialize our position
    position.zeroize();
}

void View::pushLayer(std::string path) {
    int size = layer_list.size();
    if (size >= MAX_LAYERS) {
        debug.info(Debug::Subsystem::Internal, "Can't make anymore layers on this view\n");
        return;
    }

    // get the number of layers in the list
    int layer_name = name_space | (((size + 1) & 0xFF) << 16);

    // initialize the layers's select_name to something appropriate
    Layer *l = new Layer(layer_name, path);

    // set the layer's position
    Vector dp(0, 0, layer_list.size() * 0.2);
    l->position = position + dp;
    debug.info(Debug::Subsystem::Internal, "Layer position=%s\n", l->position.print());

    // create it's nodes
    l->initNodes();

    // add it to the list
    layer_list.push_back(l);

    top = l;
}

// Removes the top level layer
void View::popLayer(void) {
    if (layer_list.size() <= 1u) {
        return;
    }

    // get the last layer...
    Layer *l = layer_list.back();

    // remove it from the list
    layer_list.pop_back();

    // delete it
    delete l;

    // set the top layer pointer again
    top = layer_list.back();
}

// Renders the view
void View::render(bool selection, SelectionState mode) {
    if (mode == NORMAL) {
        glPushMatrix();
        glTranslated(position.x, position.y, position.z);
        model->render(selection, mode);
        glPopMatrix();

        Vector text = position;
        text.z += 0.3;
        platform->display.print3DText(text, "%i", (model->select_name >> 24));
    } else if (mode == SELECTED) {
        // we used to call this...
        // model->render(selection,mode);

        // call the layers to render from the bottom up.
        for (auto layer : layer_list) {
            layer->render(selection, NORMAL);
        }

        // tell the top layer that he's got focus.
        Layer *focus = layer_list.back();
        focus->render(selection, SELECTED);
    }
}

// Allows the selection engine to select something.
bool View::choose(int name) {
    if (model->select_name == name) {
        model->IsSelected = true;
        return true;
    } else if ((VIEW_SPACE_MASK & name) == name_space) {
        if (world->selection.state == Selection::Newer) {
            // If the node belongs to the active top layer, directly process it
            if ((Layer::LAYER_SPACE_MASK & name) == layer_list.back()->name_space) {
                return layer_list.back()->choose(name);
            }
            // Otherwise, it belongs to a previous layer. Pop layers until that layer is active.
            while (layer_list.size() > 1 && (Layer::LAYER_SPACE_MASK & name) != layer_list.back()->name_space) {
                popLayer();
            }
            if ((Layer::LAYER_SPACE_MASK & name) == layer_list.back()->name_space) {
                return layer_list.back()->choose(name);
            }
        } else if (world->selection.state == Selection::ContextMenu) {
            for (auto layer : layer_list) {
                if (layer->choose(name) == true) {
                    debug.info(Debug::Subsystem::Menu, "View::choose() Menu on layer!\n");
                    world->menu = platform->ar->getActions(ACTION_LAYER);
                    return true;
                }
            }
        }
    }
    return false;
}

} // namespace hexagon
