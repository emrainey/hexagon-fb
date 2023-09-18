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

#include "hexagon/Hexagon.h"  // class's header file

// class constructor
Choreographer::Choreographer(Node *n, Choreographer_Type_e t) {
    node = n;
    setType(t);
}

Choreographer::Choreographer(Node *n) {
    node = n;
    setType(FROZEN);
}

// class destructor
Choreographer::~Choreographer() {
    // insert your code here
}

// Generic Move Interface
void Choreographer::move(void) {
    if (type == FROZEN || type >= NUM_MOVEMENT_TYPES) {
        type = FROZEN;
        return;
    }

    if (type == SPINNER) {
        // spin slowly! set the rotation vector to be spinning in somewhat
        // enchanting ways...

        Vector s(0, 0, 0.1);
        node->rotation += s;
    } else if (type == PROLATION) {
        static double dt = 0;
        dt += 0.001;
        if (dt > TWO_PI) dt = 0;
        double r = 5;
        double x = r * cos(dt);
        double y = r * sin(dt);
        Vector s(x, y, r);
        node->rotation.x = Rad2Deg(s.getAngleX()) - 90;
        node->rotation.y = Rad2Deg(s.getAngleY()) - 90;
        node->rotation.z = Rad2Deg(s.getAngleZ()) + 90;

        /*
        debug->info("dt=%lf  <x=%lf,<y=%lf,<z=%lf\n",dt,node->rotation.x,
                                                        node->rotation.y,
                                                        node->rotation.z);
        */
    } else if (type == EXPANDER) {
        Vector e(0.2, 0.2, 0);
        node->velocity += e;
    } else if (type == CONTRACTOR) {
        Vector e(-0.2, -0.2, 0);
        node->velocity += e;
    }

    node->position += node->velocity;
}

// Sets the type of Choreographer it will be from now on.
void Choreographer::setType(Choreographer_Type_e t) { type = t; }
