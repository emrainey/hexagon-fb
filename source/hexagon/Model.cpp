/*******************************************************************************
 * COPYRIGHT (C) 2003-2026, ERIK RAINEY.
 *
 * This source code has not yet been approved for release under any license
 * and as such has no warranty or fitness of purpose associated with it. Use at
 * your own discretion.
 *
 * @author Erik Rainey
 ******************************************************************************/

#include "hexagon/Hexagon.hpp"  // class's header file

extern Port *platform;

Model::Model() {
    normal = 0;
    selected = 0;
    // Default appearance (Permitted directories purple color scheme)
    appearance.face_color          = Color(0.7f, 0.1f, 0.7f, 0.3f);
    appearance.wire_color          = Color(1.0f, 1.0f, 1.0f, 0.7f);
    appearance.selected_face_color = Color(0.8f, 0.1f, 0.0f, 0.3f);
    appearance.selected_wire_color = Color(1.0f, 1.0f, 1.0f, 0.7f);
    appearance.model_name          = "";
}

Model::Model(Appearance app) {
    normal = 0;
    selected = 0;
    appearance = app;
}

Model::Model(const char *extension) {
    normal = 0;
    selected = 0;
    // Default appearance (Permitted directories purple color scheme)
    appearance.face_color          = Color(0.7f, 0.1f, 0.7f, 0.3f);
    appearance.wire_color          = Color(1.0f, 1.0f, 1.0f, 0.7f);
    appearance.selected_face_color = Color(0.8f, 0.1f, 0.0f, 0.3f);
    appearance.selected_wire_color = Color(1.0f, 1.0f, 1.0f, 0.7f);
    appearance.model_name          = "";
    loadModel(extension);
}

Model::Model(const char *extension, Appearance app) {
    normal = 0;
    selected = 0;
    appearance = app;
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
    normal = buildDownArrow(0.8, 0.8, 0.2, appearance.face_color, appearance.wire_color);
    selected = buildDownArrow(0.8, 0.8, 0.2, appearance.selected_face_color, appearance.selected_wire_color);
    info();
}

// Loads a model from the filesystem
void Model::loadModel(const char *extension) {
    // L3DS loader;
    // loader.LoadFile(fullpath);
    // int mesh_count = loader.GetMeshCount();
    // if (mesh_count != 0) {
    //     debug.info(Debug::Subsystem::Models, "There are %d meshes in the 3ds file\n", mesh_count);
    //     for (int i = 0; i < mesh_count; i++) {
    //         debug.info(Debug::Subsystem::Models, "Mesh %d has %d vertices and %d triangles\n", i,
    //                    loader.GetMesh(i).GetVertexCount(), loader.GetMesh(i).GetTriangleCount());
    //     }
    //     normal = buildMesh(loader, 0.7, 0.1, 0.7, 0.3);
    //     selected = buildMesh(loader, 1.0, 1.0, 1.0, 0.7);
    // } else {
    // couldn't find it?
    // just load the hexagon call item for now...
    normal = buildHexagon(0.8, 0.2, appearance.face_color, appearance.wire_color);
    selected = buildHexagon(0.8, 0.2, appearance.selected_face_color, appearance.selected_wire_color);
    // }
    info();
}

void Model::loadPoly() {
    normal = buildPoly(0.8, 0, appearance.face_color, appearance.wire_color);
    selected = buildPoly(0.8, 0, appearance.selected_face_color, appearance.selected_wire_color);
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
