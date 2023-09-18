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

#ifndef LAYER_H
#define LAYER_H

#include "hexagon/Hexagon.h"

// This contains the node list for display.
class Layer {
public:
    // the template list of Nodes
    std::list<Node *> node_list;
#define MAX_NODES (0xFFFF)

    // The selected node
    Node *selected;

    // usually set to the current directory of the subview
    std::string path;

    // A model of the current layer
    Model *model;

    // the location of the layer
    Vector position;

// the selection name space of this layer
#define LAYER_SPACE_MASK (0xFFFF0000)
    int name_space;

    // class constructor
    Layer(int select_name, std::string path);

    // class destructor
    ~Layer();

    // renders the layer
    void render(bool selection, Select_Type_e mode);

    // finds the OpenGL selection item
    bool choose(int name);

    // creates the default node list
    void initNodes(void);

    // adds a node to the list
    void addNode(std::string path, std::string name);

    // computes the node's new position
    Vector *placeNode(int num_nodes);
    // Moves the nodes in the current layer...
    void move();
};

#endif  // LAYER_H
