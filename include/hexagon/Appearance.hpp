/**
 * COPYRIGHT (C) 2003-2026, ERIK RAINEY.
 *
 * This source code has not yet been approved for release under any license
 * and as such has no warranty or fitness of purpose associated with it. Use at
 * your own discretion.
 *
 * @author Erik Rainey
 * @file Appearance.hpp
 */

#ifndef APPEARANCE_HPP
#define APPEARANCE_HPP

#include <string>
#include "hexagon/Color.hpp"

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
    GEAR,
    NUM_MODEL_TYPES
} Model_Type_e;

/**
 * @struct Appearance
 * @brief Holds visual properties (colors and model association) for a node.
 */
struct Appearance {
    Color face_color;
    Color wire_color;
    Color selected_face_color;
    Color selected_wire_color;
    std::string model_name;
};

/**
 * @class AppearanceLookup
 * @brief Determines the visual Appearance of a Node based on its properties.
 */
class AppearanceLookup {
public:
    /**
     * @brief Looks up and returns the appearance configuration based on hierarchy.
     * @param node_type The type of node (file, directory, etc.)
     * @param is_executable True if the node is an executable file
     * @param is_permitted True if the user has access permissions
     * @param is_hidden True if the node is pre-pended with a period
     * @return The Appearance structure containing the correct colors and model name
     */
    static Appearance getAppearance(Node_Type_e node_type, bool is_executable, bool is_permitted, bool is_hidden = false);
};

#endif    // APPEARANCE_HPP
