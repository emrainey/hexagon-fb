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

#ifndef LIGHT_H
#define LIGHT_H

#include "hexagon/Vector.h"
#include "hexagon/glut/Wrapper.h"

typedef enum { SIMPLE, SPOT, NUM_LIGHT_TYPES } Light_Type_e;

// Simple Light Class
class Light {
public:
    // the total number of lights
    static int total_num_lights;

    // our enumerated number
    int number;

    // simple light info
    Vector position;

    // if it's a spotlight then this is the direction it's pointing in.
    Vector direction;  // derived from a camera.to-camera.from subtraction

    // if it's a spot light then this is it's cutoff.
    GLfloat cutoff;

    // the lights type
    Light_Type_e type;

    // light color attributes
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];

    // class constructor
    Light(Vector p, Light_Type_e type, int total_num_lights);

    // class destructor
    ~Light();

    /*
     * If the parameter is true then the light shines if false it's turned off.
     * Regardless, it's position is set if it has moved.
     */
    void shine(bool on);
    // Sets the lights ambient, diffuse, and specular colors
    void setColorAttributes(GLfloat *a, GLfloat *d, GLfloat *s);
    // Sets the direction of the spot light
    void setDirection(Vector d);
    // Specifies the cutoff
    void setCutoff(int c);

private:
    GLenum getGLenum(void);
};

#endif  // LIGHT_H
