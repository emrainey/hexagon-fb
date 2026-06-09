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
#include <vector>
#include <cmath>

extern Port *platform;

Model::Model() {
    normal = 0;
    selected = 0;
    // Default appearance (Permitted directories purple color scheme)
    appearance.face_color = Color(0.7f, 0.1f, 0.7f, 0.3f);
    appearance.wire_color = Color(1.0f, 1.0f, 1.0f, 0.7f);
    appearance.selected_face_color = Color(0.8f, 0.1f, 0.0f, 0.3f);
    appearance.selected_wire_color = Color(1.0f, 1.0f, 1.0f, 0.7f);
    appearance.model_name = "";
    radius_factor = 1.0;
    height_factor = 1.0;
}

Model::Model(Appearance app, double radius_factor, double height_factor) {
    normal = 0;
    selected = 0;
    appearance = app;
    this->radius_factor = radius_factor;
    this->height_factor = height_factor;
}

Model::Model(const char *extension) {
    normal = 0;
    selected = 0;
    // Default appearance (Permitted directories purple color scheme)
    appearance.face_color = Color(0.7f, 0.1f, 0.7f, 0.3f);
    appearance.wire_color = Color(1.0f, 1.0f, 1.0f, 0.7f);
    appearance.selected_face_color = Color(0.8f, 0.1f, 0.0f, 0.3f);
    appearance.selected_wire_color = Color(1.0f, 1.0f, 1.0f, 0.7f);
    appearance.model_name = "";
    radius_factor = 1.0;
    height_factor = 1.0;
    loadModel(extension);
}

Model::Model(const char *extension, Appearance app, double radius_factor, double height_factor) {
    normal = 0;
    selected = 0;
    appearance = app;
    this->radius_factor = radius_factor;
    this->height_factor = height_factor;
    loadModel(extension);
}

// class destructor
Model::~Model() {
    if (normal != 0) {
        glDeleteLists(normal, 1);
    }
    if (selected != 0) {
        glDeleteLists(selected, 1);
    }
}

void Model::loadDownArrow() {
    normal = buildDownArrow(0.8 * radius_factor, 0.8 * radius_factor, 0.2 * height_factor, appearance.face_color, appearance.wire_color);
    selected =
        buildDownArrow(0.8 * radius_factor, 0.8 * radius_factor, 0.2 * height_factor, appearance.selected_face_color, appearance.selected_wire_color);
    info();
}

// Loads a model from the filesystem
void Model::loadModel(const char *extension) {
    std::string ext(extension);
    if (ext.rfind("gear_", 0) == 0) {
        int teeth = 5;
        try {
            teeth = std::stoi(ext.substr(5));
        } catch (...) {}
        normal = buildGear(0.8 * radius_factor, 0.2 * height_factor, teeth, appearance.face_color, appearance.wire_color);
        selected = buildGear(0.8 * radius_factor, 0.2 * height_factor, teeth, appearance.selected_face_color, appearance.selected_wire_color);
    } else if (ext == "folder" || ext == "removeable_drive" || ext == "remote_drive" ||
               ext == "fixed_drive" || ext == "ramdisk_drive" || ext == "cdrom_drive") {
        normal = buildHexagon(0.8 * radius_factor, 0.2 * height_factor, appearance.face_color, appearance.wire_color);
        selected = buildHexagon(0.8 * radius_factor, 0.2 * height_factor, appearance.selected_face_color, appearance.selected_wire_color);
    } else {
        // Files are represented by Cubes!
        double w = 0.8 * radius_factor;
        double d = 0.8 * radius_factor;
        double h = 0.2 * height_factor;
        normal = buildCube(w, d, h, appearance.face_color, appearance.wire_color);
        selected = buildCube(w, d, h, appearance.selected_face_color, appearance.selected_wire_color);
    }
    info();
}

void Model::loadPoly() {
    normal = buildPoly(0.8 * radius_factor, 0 * height_factor, appearance.face_color, appearance.wire_color);
    selected = buildPoly(0.8 * radius_factor, 0 * height_factor, appearance.selected_face_color, appearance.selected_wire_color);
    info();
}

void Model::info() {
    debug.info(Debug::Subsystem::Models, "Loaded Model into GL Call List (%i,%i)\n", normal, selected);
    REPORT_ERROR(info, true);
}

// Renders the model based on it's mode
void Model::render(bool selection, SelectionState mode) {
    if (selection == true) {
        IsSelected = false;
        glLoadName(select_name);
        debug.info(Debug::Subsystem::Selection, "Name,Normal,Selected={0x%08x,%d,%d}\n", select_name, normal, selected);
        REPORT_ERROR(render, true);
    }

    if (mode == NORMAL) {
        glCallList(normal);
    } else if (mode == SELECTED) {
        glCallList(selected);
    }
    REPORT_ERROR(render, true);
}

/****************************************************************************
 *        Name:
 *  Parameters:
 *     Returns:
 * Description:
 ****************************************************************************/
GLuint Model::buildHexagon(double radius, double height, Color face, Color wire) {
    GLuint shape = glGenLists(1);
    if (shape == 0) {
        REPORT_ERROR(glGenLists, false);
    }
    glNewList(shape, GL_COMPILE);
    renderHexagon(radius, height, face, wire);
    glEndList();

    REPORT_ERROR(buildHexagon, false);
    return shape;
}

void Model::renderHexagon(double radius, double height, Color face, Color wire) {
    glColor4f(face.getRedFloat(), face.getGreenFloat(), face.getBlueFloat(), face.getAlphaFloat());
    solidHexagon(radius, height);
    glColor4f(wire.getRedFloat(), wire.getGreenFloat(), wire.getBlueFloat(), wire.getAlphaFloat());
    wireHexagon(radius, height);
}

/****************************************************************************
 *        Name:
 *  Parameters:
 *     Returns:
 * Description:
 ****************************************************************************/
GLuint Model::buildPoly(double radius, double height, Color face, Color wire) {
    GLuint shape = glGenLists(1);
    if (shape == 0) {
        REPORT_ERROR(glGenLists, false);
    }
    glNewList(shape, GL_COMPILE);
    renderPoly(radius, height, face, wire);
    glEndList();
    return shape;
}

void Model::renderPoly(double radius, double height, Color face, Color wire) {
    glColor4f(face.getRedFloat(), face.getGreenFloat(), face.getBlueFloat(), face.getAlphaFloat());
    HexPoly(radius, height);
    glColor4f(wire.getRedFloat(), wire.getGreenFloat(), wire.getBlueFloat(), wire.getAlphaFloat());
    HexWirePoly(radius, height);
}

/****************************************************************************
 *        Name:
 *  Parameters:
 *     Returns:
 * Description:
 ****************************************************************************/
GLuint Model::buildDownArrow(double length, double width, double height, Color face, Color wire) {
    GLuint shape = glGenLists(1);
    if (shape == 0) {
        REPORT_ERROR(glGenLists, false);
    }
    glNewList(shape, GL_COMPILE);
    renderDownArrow(length, width, height, face, wire);
    glEndList();
    return shape;
}

void Model::renderDownArrow(double length, double width, double height, Color face, Color wire) {
    glColor4f(face.getRedFloat(), face.getGreenFloat(), face.getBlueFloat(), face.getAlphaFloat());
    solidDownArrow(length, width, height);
    glColor4f(wire.getRedFloat(), wire.getGreenFloat(), wire.getBlueFloat(), wire.getAlphaFloat());
    wireDownArrow(length, width, height);
}

/****************************************************************************
 *        Name:
 *  Parameters:
 *     Returns:
 * Description:
 ****************************************************************************/
void Model::wireHexagon(double radius, double height) {
    glBegin(GL_LINES);
    glVertex3d(radius * COS(0), radius * SIN(0), height);
    glVertex3d(radius * COS(60), radius * SIN(60), height);
    glVertex3d(radius * COS(60), radius * SIN(60), height);
    glVertex3d(radius * COS(120), radius * SIN(120), height);
    glVertex3d(radius * COS(120), radius * SIN(120), height);
    glVertex3d(radius * COS(180), radius * SIN(180), height);
    glVertex3d(radius * COS(180), radius * SIN(180), height);
    glVertex3d(radius * COS(240), radius * SIN(240), height);
    glVertex3d(radius * COS(240), radius * SIN(240), height);
    glVertex3d(radius * COS(300), radius * SIN(300), height);
    glVertex3d(radius * COS(300), radius * SIN(300), height);
    glVertex3d(radius * COS(0), radius * SIN(0), height);

    glVertex3d(radius * COS(0), radius * SIN(0), 0);
    glVertex3d(radius * COS(60), radius * SIN(60), 0);
    glVertex3d(radius * COS(60), radius * SIN(60), 0);
    glVertex3d(radius * COS(120), radius * SIN(120), 0);
    glVertex3d(radius * COS(120), radius * SIN(120), 0);
    glVertex3d(radius * COS(180), radius * SIN(180), 0);
    glVertex3d(radius * COS(180), radius * SIN(180), 0);
    glVertex3d(radius * COS(240), radius * SIN(240), 0);
    glVertex3d(radius * COS(240), radius * SIN(240), 0);
    glVertex3d(radius * COS(300), radius * SIN(300), 0);
    glVertex3d(radius * COS(300), radius * SIN(300), 0);
    glVertex3d(radius * COS(0), radius * SIN(0), 0);

    glVertex3d(radius * COS(0), radius * SIN(0), height);
    glVertex3d(radius * COS(0), radius * SIN(0), 0);

    glVertex3d(radius * COS(60), radius * SIN(60), height);
    glVertex3d(radius * COS(60), radius * SIN(60), 0);

    glVertex3d(radius * COS(120), radius * SIN(120), height);
    glVertex3d(radius * COS(120), radius * SIN(120), 0);

    glVertex3d(radius * COS(180), radius * SIN(180), height);
    glVertex3d(radius * COS(180), radius * SIN(180), 0);

    glVertex3d(radius * COS(240), radius * SIN(240), height);
    glVertex3d(radius * COS(240), radius * SIN(240), 0);

    glVertex3d(radius * COS(300), radius * SIN(300), height);
    glVertex3d(radius * COS(300), radius * SIN(300), 0);
    glEnd();
}

/****************************************************************************
 *        Name:
 *  Parameters:
 *     Returns:
 * Description:
 ****************************************************************************/
void Model::solidHexagon(double radius, double height) {
    /* top face */
    glBegin(GL_POLYGON);
    glVertex3d(radius * COS(0), radius * SIN(0), height);
    glVertex3d(radius * COS(60), radius * SIN(60), height);
    glVertex3d(radius * COS(120), radius * SIN(120), height);
    glVertex3d(radius * COS(180), radius * SIN(180), height);
    glVertex3d(radius * COS(240), radius * SIN(240), height);
    glVertex3d(radius * COS(300), radius * SIN(300), height);
    glEnd();

    /* bottom face (top face reverse order) */
    glBegin(GL_POLYGON);
    glVertex3d(radius * COS(300), radius * SIN(300), 0);
    glVertex3d(radius * COS(240), radius * SIN(240), 0);
    glVertex3d(radius * COS(180), radius * SIN(180), 0);
    glVertex3d(radius * COS(120), radius * SIN(120), 0);
    glVertex3d(radius * COS(60), radius * SIN(60), 0);
    glVertex3d(radius * COS(0), radius * SIN(0), 0);
    glEnd();

    /* upper right face */
    glBegin(GL_QUADS);
    glVertex3d(radius * COS(0), radius * SIN(0), height);
    glVertex3d(radius * COS(0), radius * SIN(0), 0);
    glVertex3d(radius * COS(60), radius * SIN(60), 0);
    glVertex3d(radius * COS(60), radius * SIN(60), height);
    glEnd();

    /* upper center face */
    glBegin(GL_QUADS);
    glVertex3d(radius * COS(60), radius * SIN(60), height);
    glVertex3d(radius * COS(60), radius * SIN(60), 0);
    glVertex3d(radius * COS(120), radius * SIN(120), 0);
    glVertex3d(radius * COS(120), radius * SIN(120), height);
    glEnd();

    /* upper left face */
    glBegin(GL_QUADS);
    glVertex3d(radius * COS(120), radius * SIN(120), height);
    glVertex3d(radius * COS(120), radius * SIN(120), 0);
    glVertex3d(radius * COS(180), radius * SIN(180), 0);
    glVertex3d(radius * COS(180), radius * SIN(180), height);
    glEnd();

    /* lower left face */
    glBegin(GL_QUADS);
    glVertex3d(radius * COS(180), radius * SIN(180), height);
    glVertex3d(radius * COS(180), radius * SIN(180), 0);
    glVertex3d(radius * COS(240), radius * SIN(240), 0);
    glVertex3d(radius * COS(240), radius * SIN(240), height);
    glEnd();

    /* lower center face */
    glBegin(GL_QUADS);
    glVertex3d(radius * COS(240), radius * SIN(240), height);
    glVertex3d(radius * COS(240), radius * SIN(240), 0);
    glVertex3d(radius * COS(300), radius * SIN(300), 0);
    glVertex3d(radius * COS(300), radius * SIN(300), height);
    glEnd();

    /* lower right face */
    glBegin(GL_QUADS);
    glVertex3d(radius * COS(300), radius * SIN(300), height);
    glVertex3d(radius * COS(300), radius * SIN(300), 0);
    glVertex3d(radius * COS(0), radius * SIN(0), 0);
    glVertex3d(radius * COS(0), radius * SIN(0), height);
    glEnd();
}

/****************************************************************************
 *        Name:
 *  Parameters:
 *     Returns:
 * Description:
 ****************************************************************************/
void Model::wireDownArrow(double length, double width, double height) {
    glBegin(GL_LINES);

    /* Top Plane */

    /* 1 - 2 */
    glVertex3d(0, length / 2, height);
    glVertex3d(width / 2, 0, height);

    /* 2 - 3 */
    glVertex3d(width / 2, 0, height);
    glVertex3d(width / 4, 0, height);

    /* 3 - 4 */
    glVertex3d(width / 4, 0, height);
    glVertex3d(width / 4, -length / 2, height);

    /* 4 - 5 */
    glVertex3d(width / 4, -length / 2, height);
    glVertex3d(-width / 4, -length / 2, height);

    /* 5 - 6 */
    glVertex3d(-width / 4, -length / 2, height);
    glVertex3d(-width / 4, 0, height);

    /* 6 - 7 */
    glVertex3d(-width / 4, 0, height);
    glVertex3d(-width / 2, 0, height);

    /* 7 - 1 */
    glVertex3d(-width / 2, 0, height);
    glVertex3d(0, length / 2, height);

    /* Bottom Plane */

    /* 1 - 2 */
    glVertex3d(0, length / 2, 0);
    glVertex3d(width / 2, 0, 0);

    /* 2 - 3 */
    glVertex3d(width / 2, 0, 0);
    glVertex3d(width / 4, 0, 0);

    /* 3 - 4 */
    glVertex3d(width / 4, 0, 0);
    glVertex3d(width / 4, -length / 2, 0);

    /* 4 - 5 */
    glVertex3d(width / 4, -length / 2, 0);
    glVertex3d(-width / 4, -length / 2, 0);

    /* 5 - 6 */
    glVertex3d(-width / 4, -length / 2, 0);
    glVertex3d(-width / 4, 0, 0);

    /* 6 - 7 */
    glVertex3d(-width / 4, 0, 0);
    glVertex3d(-width / 2, 0, 0);

    /* 7 - 1 */
    glVertex3d(-width / 2, 0, 0);
    glVertex3d(0, length / 2, 0);

    /* Side lines */

    /* 1 - 1 */
    glVertex3d(0, length / 2, height);
    glVertex3d(0, length / 2, 0);

    /* 2 - 2 */
    glVertex3d(width / 2, 0, height);
    glVertex3d(width / 2, 0, 0);

    /* 3 - 3 */
    glVertex3d(width / 4, 0, height);
    glVertex3d(width / 4, 0, 0);

    /* 4 - 4 */
    glVertex3d(width / 4, -length / 2, height);
    glVertex3d(width / 4, -length / 2, 0);

    /* 5 - 5 */
    glVertex3d(-width / 4, -length / 2, height);
    glVertex3d(-width / 4, -length / 2, 0);

    /* 6 - 6 */
    glVertex3d(-width / 4, 0, height);
    glVertex3d(-width / 4, 0, 0);

    /* 7 - 7 */
    glVertex3d(-width / 2, 0, height);
    glVertex3d(-width / 2, 0, 0);

    glEnd();
}

/****************************************************************************
 *        Name:
 *  Parameters:
 *     Returns:
 * Description:
 ****************************************************************************/
void Model::solidDownArrow(double length, double width, double height) {
    glBegin(GL_POLYGON);
    glVertex3d(0, length / 2, height);
    glVertex3d(width / 2, 0, height);
    glVertex3d(width / 4, 0, height);
    glVertex3d(width / 4, -length / 2, height);
    glVertex3d(-width / 4, -length / 2, height);
    glVertex3d(-width / 4, 0, height);
    glVertex3d(-width / 2, 0, height);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex3d(0, length / 2, 0);
    glVertex3d(width / 2, 0, 0);
    glVertex3d(width / 4, 0, 0);
    glVertex3d(width / 4, -length / 2, 0);
    glVertex3d(-width / 4, -length / 2, 0);
    glVertex3d(-width / 4, 0, 0);
    glVertex3d(-width / 2, 0, 0);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex3d(0, length / 2, height);
    glVertex3d(0, length / 2, 0);
    glVertex3d(width / 2, 0, 0);
    glVertex3d(width / 2, 0, height);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex3d(width / 2, 0, height);
    glVertex3d(width / 2, 0, 0);
    glVertex3d(width / 4, 0, 0);
    glVertex3d(width / 4, 0, height);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex3d(width / 4, 0, height);
    glVertex3d(width / 4, 0, 0);
    glVertex3d(width / 4, -length / 2, 0);
    glVertex3d(width / 4, -length / 2, height);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex3d(width / 4, -length / 2, height);
    glVertex3d(width / 4, -length / 2, 0);
    glVertex3d(-width / 4, -length / 2, 0);
    glVertex3d(-width / 4, -length / 2, height);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex3d(-width / 4, -length / 2, height);
    glVertex3d(-width / 4, -length / 2, 0);
    glVertex3d(-width / 4, 0, 0);
    glVertex3d(-width / 4, 0, height);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex3d(-width / 4, 0, height);
    glVertex3d(-width / 4, 0, 0);
    glVertex3d(-width / 2, 0, 0);
    glVertex3d(-width / 2, 0, height);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex3d(-width / 2, 0, height);
    glVertex3d(-width / 2, 0, 0);
    glVertex3d(0, length / 2, 0);
    glVertex3d(0, length / 2, height);
    glEnd();
}

// Renders a single polygon
void Model::HexPoly(double radius, double height) {
    glBegin(GL_POLYGON);
    glVertex3d(radius * COS(0), radius * SIN(0), height);
    glVertex3d(radius * COS(60), radius * SIN(60), height);
    glVertex3d(radius * COS(120), radius * SIN(120), height);
    glVertex3d(radius * COS(180), radius * SIN(180), height);
    glVertex3d(radius * COS(240), radius * SIN(240), height);
    glVertex3d(radius * COS(300), radius * SIN(300), height);
    glEnd();
}

// Renders a single polygon wire frame
void Model::HexWirePoly(double radius, double height) {
    glBegin(GL_LINES);
    glVertex3d(radius * COS(0), radius * SIN(0), height);
    glVertex3d(radius * COS(60), radius * SIN(60), height);
    glVertex3d(radius * COS(60), radius * SIN(60), height);
    glVertex3d(radius * COS(120), radius * SIN(120), height);
    glVertex3d(radius * COS(120), radius * SIN(120), height);
    glVertex3d(radius * COS(180), radius * SIN(180), height);
    glVertex3d(radius * COS(180), radius * SIN(180), height);
    glVertex3d(radius * COS(240), radius * SIN(240), height);
    glVertex3d(radius * COS(240), radius * SIN(240), height);
    glVertex3d(radius * COS(300), radius * SIN(300), height);
    glVertex3d(radius * COS(300), radius * SIN(300), height);
    glVertex3d(radius * COS(0), radius * SIN(0), height);
    glEnd();
}

GLuint Model::buildCube(double width, double depth, double height, Color face, Color wire) {
    GLuint shape = glGenLists(1);
    if (shape == 0) {
        REPORT_ERROR(glGenLists, false);
    }
    glNewList(shape, GL_COMPILE);
    renderCube(width, depth, height, face, wire);
    glEndList();

    REPORT_ERROR(buildCube, false);
    return shape;
}

void Model::renderCube(double width, double depth, double height, Color face, Color wire) {
    glColor4f(face.getRedFloat(), face.getGreenFloat(), face.getBlueFloat(), face.getAlphaFloat());
    solidCube(width, depth, height);
    glColor4f(wire.getRedFloat(), wire.getGreenFloat(), wire.getBlueFloat(), wire.getAlphaFloat());
    wireCube(width, depth, height);
}

void Model::solidCube(double w, double d, double h) {
    double hw = w / 2.0;
    double hd = d / 2.0;

    glBegin(GL_QUADS);
    // Top face (Z = h)
    glNormal3d(0.0, 0.0, 1.0);
    glVertex3d(-hw, -hd, h);
    glVertex3d( hw, -hd, h);
    glVertex3d( hw,  hd, h);
    glVertex3d(-hw,  hd, h);

    // Bottom face (Z = 0)
    glNormal3d(0.0, 0.0, -1.0);
    glVertex3d(-hw,  hd, 0.0);
    glVertex3d( hw,  hd, 0.0);
    glVertex3d( hw, -hd, 0.0);
    glVertex3d(-hw, -hd, 0.0);

    // Front face (Y = -hd)
    glNormal3d(0.0, -1.0, 0.0);
    glVertex3d(-hw, -hd, 0.0);
    glVertex3d( hw, -hd, 0.0);
    glVertex3d( hw, -hd, h);
    glVertex3d(-hw, -hd, h);

    // Back face (Y = hd)
    glNormal3d(0.0, 1.0, 0.0);
    glVertex3d( hw,  hd, 0.0);
    glVertex3d(-hw,  hd, 0.0);
    glVertex3d(-hw,  hd, h);
    glVertex3d( hw,  hd, h);

    // Left face (X = -hw)
    glNormal3d(-1.0, 0.0, 0.0);
    glVertex3d(-hw,  hd, 0.0);
    glVertex3d(-hw, -hd, 0.0);
    glVertex3d(-hw, -hd, h);
    glVertex3d(-hw,  hd, h);

    // Right face (X = hw)
    glNormal3d(1.0, 0.0, 0.0);
    glVertex3d( hw, -hd, 0.0);
    glVertex3d( hw,  hd, 0.0);
    glVertex3d( hw,  hd, h);
    glVertex3d( hw, -hd, h);
    glEnd();
}

void Model::wireCube(double w, double d, double h) {
    double hw = w / 2.0;
    double hd = d / 2.0;

    glBegin(GL_LINES);
    // Top face edges
    glVertex3d(-hw, -hd, h); glVertex3d( hw, -hd, h);
    glVertex3d( hw, -hd, h); glVertex3d( hw,  hd, h);
    glVertex3d( hw,  hd, h); glVertex3d(-hw,  hd, h);
    glVertex3d(-hw,  hd, h); glVertex3d(-hw, -hd, h);

    // Bottom face edges
    glVertex3d(-hw, -hd, 0.0); glVertex3d( hw, -hd, 0.0);
    glVertex3d( hw, -hd, 0.0); glVertex3d( hw,  hd, 0.0);
    glVertex3d( hw,  hd, 0.0); glVertex3d(-hw,  hd, 0.0);
    glVertex3d(-hw,  hd, 0.0); glVertex3d(-hw, -hd, 0.0);

    // Vertical columns
    glVertex3d(-hw, -hd, 0.0); glVertex3d(-hw, -hd, h);
    glVertex3d( hw, -hd, 0.0); glVertex3d( hw, -hd, h);
    glVertex3d( hw,  hd, 0.0); glVertex3d( hw,  hd, h);
    glVertex3d(-hw,  hd, 0.0); glVertex3d(-hw,  hd, h);
    glEnd();
}

GLuint Model::buildGear(double radius, double height, int teeth, Color face, Color wire) {
    GLuint shape = glGenLists(1);
    if (shape == 0) {
        REPORT_ERROR(glGenLists, false);
    }
    glNewList(shape, GL_COMPILE);
    renderGear(radius, height, teeth, face, wire);
    glEndList();

    REPORT_ERROR(buildGear, false);
    return shape;
}

void Model::renderGear(double radius, double height, int teeth, Color face, Color wire) {
    glColor4f(face.getRedFloat(), face.getGreenFloat(), face.getBlueFloat(), face.getAlphaFloat());
    solidGear(radius, height, teeth);
    glColor4f(wire.getRedFloat(), wire.getGreenFloat(), wire.getBlueFloat(), wire.getAlphaFloat());
    wireGear(radius, height, teeth);
}

void Model::solidGear(double radius, double height, int teeth) {
    if (teeth < 3) teeth = 3;
    int N = 5 * teeth;
    std::vector<double> x_out(N);
    std::vector<double> y_out(N);
    std::vector<double> x_in(N);
    std::vector<double> y_in(N);

    double r_outer = radius;
    double r_inner = radius * 0.75;
    double r_hole = radius * 0.3;
    double dt = std::numbers::twopi / teeth;

    for (int j = 0; j < N; ++j) {
        int tooth_index = j / 5;
        int step = j % 5;
        double fract = 0.0;
        double r = r_inner;
        switch (step) {
            case 0: fract = 0.0;  r = r_inner; break;
            case 1: fract = 0.25; r = r_inner; break;
            case 2: fract = 0.35; r = r_outer; break;
            case 3: fract = 0.65; r = r_outer; break;
            case 4: fract = 0.75; r = r_inner; break;
        }
        double angle = (tooth_index + fract) * dt;
        x_out[j] = r * cos(angle);
        y_out[j] = r * sin(angle);
        x_in[j] = r_hole * cos(angle);
        y_in[j] = r_hole * sin(angle);
    }

    // 1. Top face (Z = height)
    glBegin(GL_QUAD_STRIP);
    glNormal3d(0.0, 0.0, 1.0);
    for (int j = 0; j <= N; ++j) {
        int idx = j % N;
        glVertex3d(x_in[idx], y_in[idx], height);
        glVertex3d(x_out[idx], y_out[idx], height);
    }
    glEnd();

    // 2. Bottom face (Z = 0)
    glBegin(GL_QUAD_STRIP);
    glNormal3d(0.0, 0.0, -1.0);
    for (int j = 0; j <= N; ++j) {
        int idx = j % N;
        glVertex3d(x_out[idx], y_out[idx], 0.0);
        glVertex3d(x_in[idx], y_in[idx], 0.0);
    }
    glEnd();

    // 3. Outer teeth walls
    glBegin(GL_QUADS);
    for (int j = 0; j < N; ++j) {
        int next = (j + 1) % N;
        double dx = x_out[next] - x_out[j];
        double dy = y_out[next] - y_out[j];
        double len = sqrt(dx*dx + dy*dy);
        double nx = 0.0;
        double ny = 0.0;
        if (len > 1e-7) {
            nx = dy / len;
            ny = -dx / len;
        }
        glNormal3d(nx, ny, 0.0);
        glVertex3d(x_out[j], y_out[j], height);
        glVertex3d(x_out[j], y_out[j], 0.0);
        glVertex3d(x_out[next], y_out[next], 0.0);
        glVertex3d(x_out[next], y_out[next], height);
    }
    glEnd();

    // 4. Inner hole wall
    glBegin(GL_QUADS);
    for (int j = 0; j < N; ++j) {
        int next = (j + 1) % N;
        double dx = x_in[next] - x_in[j];
        double dy = y_in[next] - y_in[j];
        double len = sqrt(dx*dx + dy*dy);
        double nx = 0.0;
        double ny = 0.0;
        if (len > 1e-7) {
            nx = -dy / len;
            ny = dx / len;
        }
        glNormal3d(nx, ny, 0.0);
        glVertex3d(x_in[j], y_in[j], height);
        glVertex3d(x_in[next], y_in[next], height);
        glVertex3d(x_in[next], y_in[next], 0.0);
        glVertex3d(x_in[j], y_in[j], 0.0);
    }
    glEnd();
}

void Model::wireGear(double radius, double height, int teeth) {
    if (teeth < 3) teeth = 3;
    int N = 5 * teeth;
    std::vector<double> x_out(N);
    std::vector<double> y_out(N);
    std::vector<double> x_in(N);
    std::vector<double> y_in(N);

    double r_outer = radius;
    double r_inner = radius * 0.75;
    double r_hole = radius * 0.3;
    double dt = std::numbers::twopi / teeth;

    for (int j = 0; j < N; ++j) {
        int tooth_index = j / 5;
        int step = j % 5;
        double fract = 0.0;
        double r = r_inner;
        switch (step) {
            case 0: fract = 0.0;  r = r_inner; break;
            case 1: fract = 0.25; r = r_inner; break;
            case 2: fract = 0.35; r = r_outer; break;
            case 3: fract = 0.65; r = r_outer; break;
            case 4: fract = 0.75; r = r_inner; break;
        }
        double angle = (tooth_index + fract) * dt;
        x_out[j] = r * cos(angle);
        y_out[j] = r * sin(angle);
        x_in[j] = r_hole * cos(angle);
        y_in[j] = r_hole * sin(angle);
    }

    // Draw top outer loop
    glBegin(GL_LINE_LOOP);
    for (int j = 0; j < N; ++j) {
        glVertex3d(x_out[j], y_out[j], height);
    }
    glEnd();

    // Draw bottom outer loop
    glBegin(GL_LINE_LOOP);
    for (int j = 0; j < N; ++j) {
        glVertex3d(x_out[j], y_out[j], 0.0);
    }
    glEnd();

    // Draw top inner loop
    glBegin(GL_LINE_LOOP);
    for (int j = 0; j < N; ++j) {
        glVertex3d(x_in[j], y_in[j], height);
    }
    glEnd();

    // Draw bottom inner loop
    glBegin(GL_LINE_LOOP);
    for (int j = 0; j < N; ++j) {
        glVertex3d(x_in[j], y_in[j], 0.0);
    }
    glEnd();

    // Draw vertical lines for teeth and hole
    glBegin(GL_LINES);
    for (int j = 0; j < N; ++j) {
        glVertex3d(x_out[j], y_out[j], height);
        glVertex3d(x_out[j], y_out[j], 0.0);

        glVertex3d(x_in[j], y_in[j], height);
        glVertex3d(x_in[j], y_in[j], 0.0);
    }
    glEnd();
}
