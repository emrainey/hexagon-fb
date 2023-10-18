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
Node::Node() {
    // el Defaultos
    name = "A Tile";
    path = "Somewhere";
    description = "A default Node() instance";

    node_type = DEFAULT;

    // the default model;
    model = new Model("");
}

Node::Node(std::string n, std::string p, std::string d, Node_Type_e nt, Model_Type_e mt, int select_name) {
    // set our name, path and description
    name = n;
    path = p;
    description = d;

    // pass our choreographer our reference so he can fuck with us...
    choreographer = new Choreographer(this);

    // initialize our model
    node_type = nt;
    model_type = mt;
    switch (mt) {
        case DOWN_ARROW:
            model = new Model();
            model->loadDownArrow();
            break;

        case REMOVABLE_DRIVE:
            // get the model for it
            model = new Model("removeable_drive");
            break;
        case REMOTE_DRIVE:
            // get the model for it
            model = new Model("remote_drive");
            break;
        case FIXED_DRIVE:
            // get the model for it
            model = new Model("fixed_drive");
            break;
        case RAMDISK_DRIVE:
            // get the model for it
            model = new Model("ramdisk_drive");
            break;
        case CDROM_DRIVE:
            // get the model for it
            model = new Model("cdrom_drive");
            break;
        case USER_DEFINED:
            // where do we load the model from???
        default:
            int ext_idx = name.find_last_of(".");
            std::string ext = "";
            if (ext_idx != name.npos) {
                // get all characters from index to end...
                ext_idx++;  // don't try to get the actual "."
                int len = name.size() - ext_idx;
                ext = name.substr(ext_idx, len);
            }

            model = new Model(ext.c_str());  // get the default model type
            choreographer->setType(PROLATION);
            break;
    }
    model->IsSelected = false;
    model->select_name = select_name;
}

// class destructor
Node::~Node() {
    if (model) {
        delete model;
    }
}

// Renders the node via either it's selected or unselected model.
void Node::render(bool selection, SelectionState mode) {
    GL_PUSH_MATRIX();
    glTranslated(position.x, position.y, position.z);
    glRotated(rotation.x, 1, 0, 0);
    glRotated(rotation.y, 0, 1, 0);
    glRotated(rotation.z, 0, 0, 1);
    model->render(selection, mode);
    GL_POP_MATRIX();
}

// Renders the descriptive text of the object.
void Node::renderDescription(bool extra) {
    if (platform->display.renderFullName == true) {
        Vector text = position + Vector(0, 0, 1);
        if (extra == true) {
            platform->display.print3DText(text, "%s\n%s", name.c_str(), description.c_str());
        } else {
            platform->display.print3DText(text, "%s", name.c_str());
        }
    }
}

bool Node::choose(int name) {
    if (name == model->select_name) {
        debug.info(Debug::Subsystem::Selection, "%s has been selected!\n", this->name.c_str());
        return true;
    }
    return false;
}
