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

#ifndef NODE_H
#define NODE_H

#include <string>

typedef enum {
    DEFAULT = 0,
    FILE_ON_DISK_TYPE,
    FILE_REMOTE_TYPE,
    DIRECTORY_TYPE,
    DEVICE_TYPE,
    SYMLINK_TYPE,
    NUM_NODE_TYPES
} Node_Type_e;

typedef enum {
    USER_DEFINED,
    FOLDER,
    REMOVABLE_DRIVE,
    REMOTE_DRIVE,
    FIXED_DRIVE,
    RAMDISK_DRIVE,
    CDROM_DRIVE,
    DOWN_ARROW,
    NUM_MODEL_TYPES
} Model_Type_e;

// The basic object of the hexagon view system.
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

    Choreographer *choreographer;  // pointer to the class which determines
                                   // how this node moves

    // class constructor
    Node();
    Node(std::string n, std::string p, std::string d, Node_Type_e nt, Model_Type_e mt, int select_name);

    // class destructor
    ~Node();

    // Renders the node via either it's selected or unselected model.
    void render(bool selection, Select_Type_e mode);

    // Renders the descriptive text of the object.
    void renderDescription(bool extra);

    // Finds the OpenGL Selection object
    bool choose(int name);
};

#endif  // NODE_H
