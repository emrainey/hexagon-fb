/*******************************************************************************
 * COPYRIGHT (C) 2003-2026, ERIK RAINEY.
 *
 * This source code has not yet been approved for release under any license
 * and as such has no warranty or fitness of purpose associated with it. Use at
 * your own discretion.
 *
 * @author Erik Rainey
 ******************************************************************************/

#include "hexagon/Hexagon.hpp"    // class's header file
#include <filesystem>

// class constructor
Node::Node() {
    // el Defaultos
    name = "A Tile";
    path = "Somewhere";
    description = "A default Node() instance";

    node_type = DEFAULT;
    is_permitted = true;
    is_executable = false;
    appearance = AppearanceLookup::getAppearance(node_type, is_executable, is_permitted);
    radius_factor = 1.0;
    height_factor = 1.0;

    // the default model;
    model = new Model("", appearance, radius_factor, height_factor);
}

Node::Node(
    const std::string &n, const std::string &p, const std::string &d, Node_Type_e nt, Model_Type_e mt, int select_name, bool is_permitted,
    bool is_executable, double radius_factor, double height_factor
) {
    // set our name, path and description
    name = n;
    path = p;
    description = d;
    node_type = nt;
    model_type = mt;
    this->is_permitted = is_permitted;
    this->is_executable = is_executable;
    this->radius_factor = radius_factor;
    this->height_factor = height_factor;

    bool is_hidden = (name.size() > 0 && name[0] == '.' && name != "." && name != "..");
    appearance = AppearanceLookup::getAppearance(node_type, is_executable, is_permitted, is_hidden);

    // pass our choreographer our reference so he can fuck with us...
    choreographer = new Choreographer(this);

    // initialize our model
    switch (mt) {
        case DOWN_ARROW:
            model = new Model(appearance, radius_factor, height_factor);
            model->loadDownArrow();
            break;

        case FOLDER:
            // get the model for it
            model = new Model("folder", appearance, radius_factor, height_factor);
            break;

        case REMOVABLE_DRIVE:
            // get the model for it
            model = new Model("removeable_drive", appearance, radius_factor, height_factor);
            break;
        case REMOTE_DRIVE:
            // get the model for it
            model = new Model("remote_drive", appearance, radius_factor, height_factor);
            break;
        case FIXED_DRIVE:
            // get the model for it
            model = new Model("fixed_drive", appearance, radius_factor, height_factor);
            break;
        case RAMDISK_DRIVE:
            // get the model for it
            model = new Model("ramdisk_drive", appearance, radius_factor, height_factor);
            break;
        case CDROM_DRIVE:
            // get the model for it
            model = new Model("cdrom_drive", appearance, radius_factor, height_factor);
            break;
        case GEAR: {
            std::filesystem::path fullpath = std::filesystem::path(path) / name;
            uintmax_t sz = 0;
            try {
                sz = std::filesystem::file_size(fullpath);
            } catch (...) {}
            int teeth = 5;
            if (sz < 50000) teeth = 5;
            else if (sz < 250000) teeth = 6;
            else if (sz < 1000000) teeth = 7;
            else if (sz < 5000000) teeth = 8;
            else teeth = 9;
            std::string gear_ext = "gear_" + std::to_string(teeth);
            model = new Model(gear_ext.c_str(), appearance, radius_factor, height_factor);
            choreographer->setType(SPINNER);
            break;
        }
        case USER_DEFINED:
            // where do we load the model from???
        default:
            int ext_idx = name.find_last_of(".");
            std::string ext = "";
            if (ext_idx != name.npos) {
                // get all characters from index to end...
                ext_idx++;    // don't try to get the actual "."
                int len = name.size() - ext_idx;
                ext = name.substr(ext_idx, len);
            }

            model = new Model(ext.c_str(), appearance, radius_factor, height_factor);    // get the default model type
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
        Vector text = position;
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
