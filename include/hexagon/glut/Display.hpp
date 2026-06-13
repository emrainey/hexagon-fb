/**
 * COPYRIGHT (C) 2003-2026, ERIK RAINEY.
 *
 * This source code has not yet been approved for release under any license
 * and as such has no warranty or fitness of purpose associated with it. Use at
 * your own discretion.
 *
 * @author Erik Rainey
 * @file Display.hpp
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include "hexagon/Color.hpp"
#include "hexagon/Vector.hpp"
#include "hexagon/glut/Camera.hpp"

namespace hexagon {

/**
 * @class Display
 * @brief OpenGL and GLUT display and screen text renderer.
 */
class Display {
public:
    /// we need a refence to the camera object
    Camera *camera;
    int width;
    int height;
    float fps;
    int frames;

    /// Are we blending or not?
    bool blending;

    /// Are we rendering the grid or not?
    bool renderGrid;
    bool renderText;
    bool renderHelp;
    bool renderFullName;

    /// location of text
    Vector text;

    /// color of the background
    Color bg;
    Color fg;
    Color target_bg;

    /// the message to print out about our control state
    char control_message[80];

    /// our default GL font
    void *font;
    int font_height;

    /// class constructor
    Display();
    /// class destructor
    ~Display();
    /// Enable or Disable special parts of the OpenGL pipeline.
    void initOpenGL();
    /// GLUT Callback to Resize the Display
    void resize(int width, int height);
    /// GLUT Callback to Draw the Display
    void draw(void);
    /// GL call to render the scene in terms for a selection computation
    int drawSelection(int x, int y);
    /// used to print text on a screen overlay in 2D space
    int print2DText(int x, int y, char const *const, ...);
    /// used to print text in 3D Space
    void print3DText(Vector text, char const *const, ...);
    /**
     * Do any post-instatiation initialization.
     */
    void postInitialize();

private:
    void renderScreenText(void);
    void renderOverLay(void);
    void drawframe(void);
    void renderXYGrid(int xbound, int ybound);
    void frameStatistics(void);
};


} // namespace hexagon

#endif    // DISPLAY_H
