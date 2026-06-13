/**
 * COPYRIGHT (C) 2003-2026, ERIK RAINEY.
 *
 * This source code has not yet been approved for release under any license
 * and as such has no warranty or fitness of purpose associated with it. Use at
 * your own discretion.
 *
 * @author Erik Rainey
 * @file Input.hpp
 */

#ifndef INPUT_H
#define INPUT_H

namespace hexagon {

/**
 * @class Input
 * @brief GLUT mouse and keyboard input callback handler.
 */
class Input {
public:
    int lastx, lasty;
    // class constructor
    Input();
    // class destructor
    ~Input();
    // GLUT Mouse Drag Callback
    void mouseDrag(int x, int y);
    // GLUT Keyboard Callback for Special Keys
    void keyboardSpecial(int key, int x, int y);
    // GLUT Keyboard Callback for Normal Keys
    void keyboardNormal(unsigned char key, int x, int y);
    // GLUT Mouse Movement Callback
    void mouseMove(int x, int y);
    // GLUT Mouse Click Callback
    void mouseClick(int button, int state, int x, int y);
    // FreeGLUT/OpenGLUT Mouse Wheel Callback
    void mouseWheel(int wheel, int direction, int x, int y);
};  


} // namespace hexagon

#endif    // INPUT_H
