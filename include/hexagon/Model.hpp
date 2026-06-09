/**
 * COPYRIGHT (C) 2003-2026, ERIK RAINEY.
 *
 * This source code has not yet been approved for release under any license
 * and as such has no warranty or fitness of purpose associated with it. Use at
 * your own discretion.
 *
 * @author Erik Rainey
 * @file Model.hpp
 */

#ifndef MODEL_H
#define MODEL_H

#include "hexagon/Appearance.hpp"
#include "hexagon/glut/Wrapper.hpp"

typedef enum {
    NORMAL,
    SELECTED,
    NUM_SELECT_TYPES
} SelectionState;

/**
 * @class Model
 * @brief This is the wrapper class around loading 3ds models from the FileSystem.
 */
class Model {
public:
    // the normal and selected Call List items
    GLuint normal;
    GLuint selected;

    // this is the GL Name of the model used during rendering and selection
    GLuint select_name;

    // a bool to keep around to determine if the model has been selected.
    bool IsSelected;

    // the visual appearance configuration
    Appearance appearance;

    double radius_factor;
    double height_factor;

    // class constructors
    Model();
    Model(Appearance appearance, double radius_factor = 1.0, double height_factor = 1.0);
    Model(const char *extension);
    Model(const char *extension, Appearance appearance, double radius_factor = 1.0, double height_factor = 1.0);

    // class destructor
    ~Model();

    // Loads a model from the filesystem
    void loadModel(const char *extension);

    // loads the Up Arrow Object
    void loadDownArrow();

    // loads a hexagonal polygon with wire outline
    void loadPoly();

    // a little debug
    void info();

    // Renders the model based on it's mode
    void render(bool selection, SelectionState mode);

    // Dynamic color parameterized shape builders
    static GLuint buildHexagon(double radius, double height, Color face, Color wire);
    static GLuint buildDownArrow(double length, double width, double height, Color face, Color wire);
    static GLuint buildPoly(double radius, double height, Color face, Color wire);
    static GLuint buildCube(double width, double depth, double height, Color face, Color wire);

    static void renderHexagon(double radius, double height, Color face, Color wire);
    static void renderDownArrow(double length, double width, double height, Color face, Color wire);
    static void renderPoly(double radius, double height, Color face, Color wire);
    static void renderCube(double width, double depth, double height, Color face, Color wire);

    static void wireHexagon(double radius, double height);
    static void solidHexagon(double radius, double height);
    static void wireDownArrow(double length, double width, double height);
    static void solidDownArrow(double length, double width, double height);
    static void HexPoly(double radius, double height);
    static void HexWirePoly(double radius, double height);
    static void wireCube(double width, double depth, double height);
    static void solidCube(double width, double depth, double height);
};

#endif    // MODEL_H
