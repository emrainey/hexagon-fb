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

#include "hexagon/Hexagon.h"

// define a pointer to the Port object
extern Port *platform;

// class constructor
Camera::Camera() {
    debug->info(Debug::Subsystem::Trace, "Camera()\n");
    init();
}

// class destructor
Camera::~Camera() {
    // insert your code here
    debug->info(Debug::Subsystem::Trace, "~Camera()\n");
}

void Camera::place(void) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, aspect, 1, 10000000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(from.x, from.y, from.z, to.x, to.y, to.z, up.x, up.y, up.z);
}

// This computes the new location of the camera.
void Camera::move(void) {
    // the camera should worry about it's own movement,
    // if we should move...
    Vector pan = dest - to;
    if (pan.abs() > 0) {
        // debug->info("dest.print()=%s\n",dest.print());
        // debug->info("to.print()=%s\n",to.print());
        // debug->info("pan.abs()=%lf\n",pan.abs());

        Vector dv = pan;
        double d = pan.abs();

        if (d < 0.1)  // if we are close then just jump it.
        {
            to = dest;
        } else if (d < 1.0)  // if the to is "close" to the destination then slow down
        {
            dv *= 0.1;
            to = dv + to;
        } else if (d > 1.0)  // if the to is far away from the destination the move
                             // faster
        {
            dv *= 0.3;
            to = dv + to;
        }
    }

    switch (mode) {
        case Camera::Type::Spherical:
            // now we figure out the from vector
            // take the "to" location and add the spherical translated
            // coordinates
            from.x = to.x + (radius * cos(PI / 2 - phi) * cos(theta));
            from.y = to.y + (radius * cos(PI / 2 - phi) * sin(theta));
            from.z = to.z + (radius * cos(phi));
            break;
        case Camera::Type::Free:
            break;
        default:
            break;
    }
}

// GLUT Callback for doing object selection
void Camera::selection(int x, int y) {
    GLint viewport[4];

    /* setup the viewport */
    glGetIntegerv(GL_VIEWPORT, viewport);

    if (debug->state == true) {
        debug->info(Debug::Subsystem::Selection, "Clicked at x,y = { %d, %d }\n", x, y);
    }

    /********************************************************************/
    /* For some reason the OpenGL selection is not working like these   */
    /* books say it does. This hack fixes it, but doesn't make alot of  */
    /* sense. It's like the Y value of where I'm clicking is off by the */
    /* inverse of the calculation below. Imagine a horizontal line half */
    /* way down the window, disecting the window in half. This is the   */
    /* mirror line. If this could be considered the X axis then the Y   */
    /* value that it appears to select at is reflected around the X     */
    /* axis. This equation reflects it before hand so the selection     */
    /* will work correctly. -- Kudos to the person who solves this one! */
    /********************************************************************/
    y = (viewport[3] / 2 - y) + (viewport[3] / 2);

    if (debug->state == true) {
        debug->info(Debug::Subsystem::Selection, "Altered Clicked at x,y = { %d, %d }\n", x, y);

        debug->info(Debug::Subsystem::Selection, "Viewport = { %d, %d, %d, %d }\n", viewport[0], viewport[1],
                    viewport[2], viewport[3]);
        debug->flush();
    }

    // I have no real idea what any of this does....
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glRenderMode(GL_SELECT);
    glLoadIdentity();
    gluPickMatrix(x, y, 2, 2, viewport);
    gluPerspective(fov, aspect, 1, 10000000);
    gluLookAt(from.x, from.y, from.z, to.x, to.y, to.z, up.x, up.y, up.z);
}

// Re-Initializes the camera.
void Camera::init(void) {
    debug->info(Debug::Subsystem::Trace, "Camera.init()\n");

    mode = Camera::Type::Spherical;
    radius = 20;
    theta = 3 * PI / 2;
    phi = PHI_MIN;

    if (platform != NULL)
        aspect = platform->display.width / platform->display.height;
    else
        aspect = 0;

    fov = 40;
    from.zeroize();
    to.zeroize();
    dest.zeroize();
    up.zeroize();
    // set our up direction...
    up.z = 1;
}

char *Camera::printInfo(void) {
    static char info[160];
    memset(info, 0, sizeof(info));
    sprintf(info,
            "Camera: \n"
            "From=(%.3lf,%.3lf,%.3lf)\n"
            //"To=(%.3lf,%.3lf,%.3lf)\n"
            "Radius=%.3lf\tTheta=%.3lf\tPhi=%.3lf\n",
            from.x, from.y, from.z,
            // to.x,to.y,to.z,
            radius, theta, phi);
    return info;
}

// Freezes the camera where is it.
void Camera::freeze(void) { /* TODO (#1#): Implement Camera::freeze() */
}

// Unfreezes the camera
void Camera::unfreeze(void) { /* TODO (#1#): Implement Camera::unfreeze() */
}

// Sets the new distance
void Camera::setDistance(float d) {
    radius = d;
    if (radius < MIN_ZOOM)
        radius = MIN_ZOOM;
    else if (radius > MAX_ZOOM)
        radius = MAX_ZOOM;

    if (radius < NAMING_DISTANCE)
        platform->display.renderFullName = true;
    else
        platform->display.renderFullName = false;
}

// Alters the distance of the camera.
void Camera::alterDistance(float delta) { setDistance(radius + delta); }
