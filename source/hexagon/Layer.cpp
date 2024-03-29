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
Layer::Layer(int select_name, std::string p) {
    model = new Model("");
    model->loadPoly();
    model->select_name = select_name;
    name_space = select_name;
    path = p;
    position.zeroize();
}

// class destructor
Layer::~Layer() {
    // insert your code here
}

void Layer::render(bool selection, SelectionState mode) {
    if (mode == SELECTED) {
        for (auto node : node_list) {
            node->render(selection, NORMAL);
        }
        for (auto node : node_list) {
            node->renderDescription(platform->display.renderText);
        }
    } else if (mode == NORMAL) {
        glPushMatrix();
        glTranslated(position.x, position.y, position.z);
        model->render(selection, mode);
        glPopMatrix();
    }
}

bool Layer::choose(int name) {
    if (model->select_name == name) {
        model->IsSelected = true;
        return true;
    } else if ((LAYER_SPACE_MASK & name) == name_space) {
        // otherwise check all of our nodes in the current view
        for (auto node : node_list) {
            if (node->choose(name) == true) {
                // we have found the node!
                // process this selection!
                selected = node;

                if (selected->node_type == DIRECTORY_TYPE) {
                    debug.info(Debug::Subsystem::Internal, "This node is a directory!\n");

                    if (world->selection.state == Selection::Newer) {
                        if (selected->model_type == DOWN_ARROW)
                            world->focus->popLayer();
                        else {
                            std::string fullpath;
                            if (path == ROOT_PATH)
                                fullpath = path + node->name;
                            else
                                fullpath = path + SYSTEM_DELIMITER + node->name;
                            world->focus->pushLayer(fullpath);
                        }
                    } else if (world->selection.state == Selection::ContextMenu) {
                        if (selected->model_type == DOWN_ARROW) {
                            debug.info(Debug::Subsystem::Menu, "Layer::choose() Menu on up!\n");
                            // call the up menu...
                            world->menu = platform->ar->getActions(ACTION_UP);
                        } else {
                            debug.info(Debug::Subsystem::Menu, "Layer::choose() Menu on directory!\n");
                            // create a new one...
                            world->menu = platform->ar->getActions(ACTION_DIRECTORY);
                        }
                    }
                } else {
                    debug.info(Debug::Subsystem::Internal, "This node is a file!\n");
                    if (world->selection.state == Selection::Newer) {
                        platform->shell->initialize("open", path, selected->name);
                        platform->shell->execute();
                    } else if (world->selection.state == Selection::ContextMenu) {
                        // create a context menu and display it...
                        debug.info(Debug::Subsystem::Menu, "Layer::choose() Menu on file!\n");
                        world->menu = platform->ar->getActions(selected->name);
                    }
                }

                debug.info(Debug::Subsystem::Internal, "Node 0x%08x selected\n", name);
                return true;
            }
        }
    }
    return false;
}

void Layer::initNodes(void) {
    // ok, we're going to scan for the directory, get a list back, then ask
    // for some info on each item then create the node

    // add the Down Arrow
    std::string down = "Down";
    addNode(path, down);

    std::list<std::string> *others = platform->fs->getDirectoryList(path);
    if (others == NULL) {
        // we have a serious problem...
        return;
    }

    for (auto entry : *others) {
        addNode(path, entry);
    }
}

void Layer::addNode(std::string path, std::string name) {
    int size = node_list.size();
    if (size > MAX_NODES) return;

    // alright... if the size() == 0 then add the Up Arrow Node
    Node *n = NULL;
    if (size == 0) {
        std::string description("Click this to go back down the stack");
        n = new Node(name, "..", description, DIRECTORY_TYPE, DOWN_ARROW, name_space | 1);
        n->position = position + *(new Vector(0, 0, 1));
    } else  // else there are already 1+ objects in the list
    {
        // and a description
        std::string description = platform->fs->getMetaData(path, name);

        // and we have to get it's type from the file system
        Node_Type_e type = platform->fs->getNodeType(path, name);

        // And we have to get it's model type (if it's a device)
        Model_Type_e model_type = platform->fs->getModelType(path, name);

        // tell the node what it's selection number is
        int select_name = name_space | (size + 1);

        // declare a new node...
        n = new Node(name, path, description, type, model_type, select_name);

        Vector *dp = placeNode(size - 1);
        n->position = position + *dp;  // place it down
    }

    // add the node...
    node_list.push_back(n);
}

// this is going to translate the number into a location vector
Vector *Layer::placeNode(int num_nodes) {
    // aw-ight, we need to calculate the place for a node to go.
    double radius = 2;
    double div = 0.333333334;
    double dt = 0;

    while (num_nodes--) {
        dt += div / radius;
        if (dt >= 1) {
            radius += 2;
            dt = 0;
        }
    }

    double x = radius * cos(2 * PI * dt);
    double y = radius * sin(2 * PI * dt);
    double z = 1;
    return new Vector(x, y, z);
}

// Moves the nodes in the current layer...
void Layer::move() {
    for (auto lit : node_list) {
        lit->choreographer->move();
    }
}
