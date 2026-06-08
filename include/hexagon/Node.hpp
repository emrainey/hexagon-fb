/**
 * COPYRIGHT (C) 2003-2026, ERIK RAINEY.
 *
 * This source code has not yet been approved for release under any license
 * and as such has no warranty or fitness of purpose associated with it. Use at
 * your own discretion.
 *
 * @author Erik Rainey
 * @file Node.hpp
 */

#ifndef NODE_H
#define NODE_H

#include <string>

#include "hexagon/Appearance.hpp"
#include "hexagon/Choreographer.hpp"
#include "hexagon/Vector.hpp"

/**
 * @class Node
 * @brief The basic object of the hexagon view system.
 */
class Node {
public:
    Node_Type_e node_type;    // what type of node is it? A file
    Model_Type_e model_type;  // what type of model should use?

    Vector position;  // it's current position
    Vector velocity;  // it's current velocity
    Vector rotation;  // it's current rotation about each axis
                      //(each rotation is right-handed)

    // the pointer to our model
    Model *model;

    // the name of the node
    std::string name;

    // a brief description
    std::string description;

    // the file directory path
    std::string path;

    // permission check for directories
    bool is_permitted;

    // executable check for files
    bool is_executable;

    // the visual appearance configuration
    Appearance appearance;

    double radius_factor;
    double height_factor;

    Choreographer *choreographer;  // pointer to the class which determines
                                   // how this node moves

    Node();
    Node(const std::string &n, const std::string &p, const std::string &d, Node_Type_e nt, Model_Type_e mt, int select_name, bool is_permitted = true, bool is_executable = false, double radius_factor = 1.0, double height_factor = 1.0);

    // class destructor
    ~Node();

    // Renders the node via either it's selected or unselected model.
    void render(bool selection, SelectionState mode);

    // Renders the descriptive text of the object.
    void renderDescription(bool extra);

    // Finds the OpenGL Selection object
    bool choose(int name);
};

#endif  // NODE_H
