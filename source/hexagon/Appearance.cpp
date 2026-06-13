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

namespace hexagon {

Appearance AppearanceLookup::getAppearance(Node_Type_e node_type, bool is_executable, bool is_permitted, bool is_hidden) {
    Appearance app;

    // 1. Lack of permission (top level precedence)
    if (!is_permitted) {
        app.face_color = Color(0.4f, 0.0f, 0.0f, 0.8f);             // Dark Red
        app.wire_color = Color(1.0f, 1.0f, 1.0f, 0.7f);             // White
        app.selected_face_color = Color(0.6f, 0.0f, 0.0f, 0.9f);    // Richer Red
        app.selected_wire_color = Color(1.0f, 1.0f, 1.0f, 0.7f);    // White
        app.model_name = "";
    }
    // 2. Directory type
    else if (node_type == DIRECTORY_TYPE) {
        app.face_color = Color(0.7f, 0.1f, 0.7f, 0.8f);             // Purple
        app.wire_color = Color(1.0f, 1.0f, 1.0f, 0.7f);             // White
        app.selected_face_color = Color(0.8f, 0.1f, 0.0f, 0.8f);    // Reddish-orange
        app.selected_wire_color = Color(1.0f, 1.0f, 1.0f, 0.7f);    // White
        app.model_name = "folder";
    }
    // 3. Executable file
    else if (is_executable) {
        app.face_color = Color(0.0f, 0.2f, 0.6f, 0.8f);             // Deep Royal Blue
        app.wire_color = Color(1.0f, 1.0f, 1.0f, 0.7f);             // White
        app.selected_face_color = Color(0.1f, 0.4f, 0.8f, 0.9f);    // Bright Royal Blue
        app.selected_wire_color = Color(1.0f, 1.0f, 1.0f, 0.7f);    // White
        app.model_name = "executable";
    }
    // 4. Normal files / other types (fallback)
    else {
        app.face_color = Color(0.1f, 0.6f, 0.8f, 0.8f);             // Light Blue/Cyan
        app.wire_color = Color(1.0f, 1.0f, 1.0f, 0.7f);             // White
        app.selected_face_color = Color(0.2f, 0.8f, 1.0f, 0.9f);    // Ice Blue
        app.selected_wire_color = Color(1.0f, 1.0f, 1.0f, 0.7f);    // White
        app.model_name = "";
    }

    if (is_hidden) {
        // Dim faces and wireframe, and make them much more translucent (reduce alpha)
        app.face_color.setRedFloat(app.face_color.getRedFloat() * 0.4f);
        app.face_color.setGreenFloat(app.face_color.getGreenFloat() * 0.4f);
        app.face_color.setBlueFloat(app.face_color.getBlueFloat() * 0.4f);
        app.face_color.setAlphaFloat(app.face_color.getAlphaFloat() * 0.4f);

        app.wire_color.setRedFloat(app.wire_color.getRedFloat() * 0.4f);
        app.wire_color.setGreenFloat(app.wire_color.getGreenFloat() * 0.4f);
        app.wire_color.setBlueFloat(app.wire_color.getBlueFloat() * 0.4f);
        app.wire_color.setAlphaFloat(app.wire_color.getAlphaFloat() * 0.4f);

        app.selected_face_color.setRedFloat(app.selected_face_color.getRedFloat() * 0.5f);
        app.selected_face_color.setGreenFloat(app.selected_face_color.getGreenFloat() * 0.5f);
        app.selected_face_color.setBlueFloat(app.selected_face_color.getBlueFloat() * 0.5f);
        app.selected_face_color.setAlphaFloat(app.selected_face_color.getAlphaFloat() * 0.5f);

        app.selected_wire_color.setRedFloat(app.selected_wire_color.getRedFloat() * 0.5f);
        app.selected_wire_color.setGreenFloat(app.selected_wire_color.getGreenFloat() * 0.5f);
        app.selected_wire_color.setBlueFloat(app.selected_wire_color.getBlueFloat() * 0.5f);
        app.selected_wire_color.setAlphaFloat(app.selected_wire_color.getAlphaFloat() * 0.5f);
    }

    return app;
}

} // namespace hexagon
