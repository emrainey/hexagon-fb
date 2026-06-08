/*******************************************************************************
 * COPYRIGHT (C) 2003-2026, ERIK RAINEY.
 *
 * This source code has not yet been approved for release under any license
 * and as such has no warranty or fitness of purpose associated with it. Use at
 * your own discretion.
 *
 * @author Erik Rainey
 * @file Appearance.cpp
 ******************************************************************************/

#include "hexagon/Appearance.hpp"

Appearance AppearanceLookup::getAppearance(Node_Type_e node_type, bool is_executable, bool is_permitted) {
    Appearance app;

    // 1. Lack of permission (top level precedence)
    if (!is_permitted) {
        app.face_color = Color(0.4f, 0.0f, 0.0f, 0.3f);             // Dark Red
        app.wire_color = Color(1.0f, 1.0f, 1.0f, 0.7f);             // White
        app.selected_face_color = Color(0.6f, 0.0f, 0.0f, 0.5f);    // Richer Red
        app.selected_wire_color = Color(1.0f, 1.0f, 1.0f, 0.7f);    // White
        app.model_name = "";
        return app;
    }

    // 2. Directory type
    if (node_type == DIRECTORY_TYPE) {
        app.face_color = Color(0.7f, 0.1f, 0.7f, 0.3f);             // Purple
        app.wire_color = Color(1.0f, 1.0f, 1.0f, 0.7f);             // White
        app.selected_face_color = Color(0.8f, 0.1f, 0.0f, 0.3f);    // Reddish-orange
        app.selected_wire_color = Color(1.0f, 1.0f, 1.0f, 0.7f);    // White
        app.model_name = "folder";
        return app;
    }

    // 3. Executable file
    if (is_executable) {
        app.face_color = Color(0.6f, 0.5f, 0.0f, 0.3f);             // Dark Yellow
        app.wire_color = Color(1.0f, 1.0f, 1.0f, 0.7f);             // White
        app.selected_face_color = Color(0.8f, 0.7f, 0.0f, 0.5f);    // Bright Yellow
        app.selected_wire_color = Color(1.0f, 1.0f, 1.0f, 0.7f);    // White
        app.model_name = "executable";
        return app;
    }

    // 4. Normal files / other types (fallback)
    app.face_color = Color(0.1f, 0.6f, 0.8f, 0.3f);             // Light Blue/Cyan
    app.wire_color = Color(1.0f, 1.0f, 1.0f, 0.7f);             // White
    app.selected_face_color = Color(0.2f, 0.8f, 1.0f, 0.5f);    // Ice Blue
    app.selected_wire_color = Color(1.0f, 1.0f, 1.0f, 0.7f);    // White
    app.model_name = "";
    return app;
}
