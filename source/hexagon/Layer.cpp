/*******************************************************************************
 * COPYRIGHT (C) 2003-2026, ERIK RAINEY.
 *
 * This source code has not yet been approved for release under any license
 * and as such has no warranty or fitness of purpose associated with it. Use at
 * your own discretion.
 *
 * @author Erik Rainey
 ******************************************************************************/

#include <cmath>
#include <filesystem>
#include <format>
#include <numbers>
#ifndef _WIN32
#include <unistd.h>
#endif

#include <fstream>
#include <cstdint>
#include "hexagon/Hexagon.hpp"    // class's header file

static bool isNativeExecutable(const std::filesystem::path& fullpath) {
    if (!std::filesystem::is_regular_file(fullpath)) return false;
    std::ifstream file(fullpath, std::ios::binary);
    if (!file) return false;
    uint32_t magic = 0;
    file.read(reinterpret_cast<char*>(&magic), sizeof(magic));
    if (file.gcount() < 4) return false;
    // Mach-O: 0xFEEDFACE, 0xFEEDFACF, 0xCAFEBABE (FAT) and byte-swapped variants
    // ELF: 0x7F454C46 and byte-swapped variant
    return (magic == 0xFEEDFACE || magic == 0xCEFAEDFE ||
            magic == 0xFEEDFACF || magic == 0xCFFAEDFE ||
            magic == 0xCAFEBABE || magic == 0xBEBAFECA ||
            magic == 0x7F454C46 || magic == 0x464C457F);
}

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
                            std::string fullpath = (std::filesystem::path(path) / node->name).string();
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

    try {
        std::filesystem::path p(path);
        if (std::filesystem::exists(p) && std::filesystem::is_directory(p)) {
            for (const auto &entry : std::filesystem::directory_iterator(p)) {
                addNode(path, entry.path().filename().string());
            }
        }
    } catch (const std::filesystem::filesystem_error &e) {
        debug.info(Debug::Subsystem::Error, "Filesystem error scanning directory: %s\n", e.what());
    }
}

void Layer::addNode(std::string path, std::string name) {
    int size = node_list.size();
    if (size > MAX_NODES) return;

    // alright... if the size() == 0 then add the Up Arrow Node
    Node *n = NULL;
    if (size == 0) {
        std::string description("Click this to go back down the stack");
        n = new Node(name, "..", description, DIRECTORY_TYPE, DOWN_ARROW, name_space | 1, true, false);
        n->position = position + *(new Vector(0, 0, 1));
    } else    // else there are already 1+ objects in the list
    {
        std::string description = "Unknown";
        Node_Type_e type = Node_Type_e::DEFAULT;
        Model_Type_e model_type = Model_Type_e::USER_DEFINED;
        bool is_permitted = true;
        bool is_executable = false;
        double radius_factor = 1.0;
        double height_factor = 1.0;

        try {
            std::filesystem::path fullpath = std::filesystem::path(path) / name;
            if (std::filesystem::is_directory(fullpath)) {
                description = "Directory";
                type = DIRECTORY_TYPE;
                model_type = FOLDER;
#ifndef _WIN32
                if (access(fullpath.string().c_str(), R_OK | X_OK) != 0) {
                    is_permitted = false;
                }
#endif
            } else if (std::filesystem::is_regular_file(fullpath)) {
                auto sz = std::filesystem::file_size(fullpath);
                description = (sz < 1024) ? std::format("{} bytes", sz) : std::format("{} kb", sz / 1024);
                type = FILE_ON_DISK_TYPE;
                model_type = USER_DEFINED;
                height_factor = 1.0 + std::log10(static_cast<double>(sz) + 1.0) * 0.5;
#ifndef _WIN32
                if (access(fullpath.string().c_str(), R_OK) != 0) {
                    is_permitted = false;
                }
                if (access(fullpath.string().c_str(), X_OK) == 0) {
                    is_executable = true;
                }
                if (isNativeExecutable(fullpath)) {
                    model_type = Model_Type_e::GEAR;
                }
#endif
            } else if (std::filesystem::is_symlink(fullpath)) {
                description = "Symbolic Link";
                type = SYMLINK_TYPE;
                model_type = USER_DEFINED;
#ifndef _WIN32
                if (access(fullpath.string().c_str(), R_OK) != 0) {
                    is_permitted = false;
                }
                if (access(fullpath.string().c_str(), X_OK) == 0) {
                    is_executable = true;
                }
#endif
            }
        } catch (const std::filesystem::filesystem_error &) {
            description = "Unavailable";
            is_permitted = false;
        }

        // tell the node what it's selection number is
        int select_name = name_space | (size + 1);

        // declare a new node...
        n = new Node(name, path, description, type, model_type, select_name, is_permitted, is_executable, radius_factor, height_factor);

        Vector *dp = placeNode(size - 1);
        n->position = position + *dp;    // place it down
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

    double x = radius * cos(2 * std::numbers::pi * dt);
    double y = radius * sin(2 * std::numbers::pi * dt);
    double z = 1;
    return new Vector(x, y, z);
}

// Moves the nodes in the current layer...
void Layer::move() {
    for (auto lit : node_list) {
        lit->choreographer->move();
    }
}
