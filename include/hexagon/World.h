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

#ifndef WORLD_H
#define WORLD_H

#include <string>

typedef enum { SELECTION_OLD = 0, SELECTION_NEW, SELECTION_CONTEXT_MENU } Selection_Mode_e;

typedef struct {
    Selection_Mode_e state;
    int x;
    int y;
} Selection_Point_t;

/**
 * The World contains all the information to render with.
 * This class contains the view list which in turn contains a layer stack
 * which in turn contains a node list.
 */
class World {
public:
    // the list of all views
    std::list<View *> view_list;
#define MAX_VIEWS (61)

    // the root view (the one in the middle)
    View *root;

    // the view currently selected
    View *focus;

    // the view with empty slots to be filled
    View *unfull;

    // The last view added
    View *last;

    // the next index on unfull to fill
    int next_fill;

    // the number of total views
    int num_total_views;

    // the point where the selection occured!
    Selection_Point_t selection;

    // the list of lights in the world.
    std::list<Light *> lights;

    // Our default menu...
    Menu *menu;

    // class constructor
    World(int n, std::string *start_view);

    // class destructor
    ~World();

    // Renders the world!
    void render(bool selection);

    // allows the selection routines to notify the object who is selected
    // that he is so.
    void choose(int name);

    // add a new view to the world
    void addView(void);

    // determines the next direction to go for the next unfull.
    int getNextPosition(int view_num);

    // adds a light to the view with default parameters
    void addLight(Vector p);

    // adds a preconfigured Light
    void addLight(Light *l);

    // Turns on or off lights.
    void shine(bool on);

    // Creates a spot light based on the camera's view
    void addSpotLight();

private:
    // adds the view to the world at the right coordinates
    void placeView(View *v, int index);

    // based on a direction, returns a Vector
    Vector *direction(int direction);
};

#endif  // WORLD_H
