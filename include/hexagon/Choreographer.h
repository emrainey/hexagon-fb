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

#ifndef CHOREOGRAPHER_H
#define CHOREOGRAPHER_H

typedef enum { FROZEN = 0, SPINNER, PROLATION, EXPANDER, CONTRACTOR, NUM_MOVEMENT_TYPES } Choreographer_Type_e;

class Node;

// No description
class Choreographer {
public:
    // this will define which choregrapher it will use...
    Choreographer_Type_e type;

    // This is the node we are moving...
    Node *node;

    // class constructor
    Choreographer(Node *n, Choreographer_Type_e type);
    Choreographer(Node *n);
    // class destructor
    ~Choreographer();
    // Generic Move Interface
    void move(void);
    // Sets the type of Choreographer it will be from now on.
    void setType(Choreographer_Type_e t);
};

#endif  // CHOREOGRAPHER_H
