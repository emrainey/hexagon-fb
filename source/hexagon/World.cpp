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

#define SIDES (6)  // the only place where we should define the number of sides
#define PREV(i) ((i - 1 < 0) ? (SIDES - 1) : (i - 1))
#define NEXT(i) ((i + 1) % SIDES)
#define INV(i) ((i + (SIDES / 2)) % SIDES)

// the default colors
static GLfloat light_ambient[] = {0.5, 0.5, 0.5, 1};
static GLfloat light_diffuse[] = {1, 1, 1, 1};
static GLfloat light_specular[] = {1, 1, 1, 1};

// class constructor
World::World(int num, std::string *start_view) {
    debug.info(Debug::Subsystem::Trace, "World((Views)%i)\n", num);
    num_total_views = num;

    // initialize our pointers
    root = NULL;
    unfull = NULL;
    focus = NULL;
    last = NULL;

    // create the requested number of views
    for (int i = 0; i < num_total_views; i++) addView();

    selection.state = Selection::Old;

    menu = NULL;

    // add the only light...
    addLight(platform->camera.from);
}

void World::addView(void) {
    if (view_list.size() >= MAX_VIEWS) return;

    int size = view_list.size();

    // this limits us to having < 256 views
    int their_name_space = ((size + 1) & 0xFF) << 24;

    // create a new view
    View *v = new View(their_name_space);

    // if this is first view...
    if (root == NULL) {
        // initialize all the pointers
        unfull = focus = root = last = v;

        // set the focus of the camera to the new view
        platform->camera.dest = v->position;
        platform->camera.to = v->position;

        // the next fill direction needs to be initialized
        next_fill = 0;
    } else {
        // get the next direction of a view, using the last index
        int d = getNextPosition(size - 1);

        // convert the direction to a vector
        Vector *vec = direction(d);

        // set the position of the view
        v->position = last->position + *vec;

        debug.info(Debug::Subsystem::Internal, "new view @ %s\n", v->position.print());
    }

    // add a layer to the view
    v->pushLayer(ROOT_PATH);

    // put this on the list
    view_list.push_back(v);

    // set up the last pointer
    last = v;
}

// class destructor
World::~World() {
    std::list<View *>::iterator lit = view_list.begin();
    while (lit != view_list.end()) {
        delete (*lit);
    }
}

// Renders the world!
void World::render(bool selection) {
    // cycle through all the view and render them, if the view is not the focus
    // then set it's mode to NORMAL
    std::list<View *>::iterator lit = view_list.begin();
    while (lit != view_list.end()) {
        if ((*lit) == focus)
            ;  // do nothing, this is to fix the depth problem
        else
            (*lit)->render(selection, NORMAL);
        lit++;
    }

    // now render the focus's item
    focus->render(selection, SELECTED);
}

// find the selected object and set it's model's IsSelected to true
void World::choose(int name) {
    debug.info(Debug::Subsystem::Trace, "+World::choose(0x%08x)\n", name);
    debug.info(Debug::Subsystem::Menu, "selection.state=%i\n", selection.state);

    std::list<View *>::iterator lit = view_list.begin();
    while (lit != view_list.end()) {
        if ((*lit)->choose(name) == true) {
            focus = (*lit);
            debug.info(Debug::Subsystem::Trace, "-World::choose(0x%08x) [2]\n", name);
            return;
        }
        lit++;
    }

    // nothing was chosen so...

    if (selection.state == Selection::ContextMenu) {
        debug.info(Debug::Subsystem::Menu, "menu is %snull\n", (menu ? "not " : ""));
        menu = platform->ar->getActions(ACTION_DEFAULT);
    }

    debug.info(Debug::Subsystem::Trace, "-World::choose(0x%08x)\n", name);
}

// v is the view we are adding
// unfull is the view that is not yet neighbor-full
// next_fill is the next neighbor to add
void World::placeView(View *v, int index) {
    debug.info(Debug::Subsystem::Trace, "World::placeView(%i)\n", v);
    debug.info(Debug::Subsystem::Internal, "\tnext_fill=%i\n", next_fill);

    // create the vector to the new View
    Vector *vec = direction(next_fill);
    debug.info(Debug::Subsystem::Internal, "\tvec->abs()=%lf\n", vec->abs());

    // add it to the position of the unfull to get the added Views
    // new position
    v->position = unfull->position + *vec;

    // debug.info("v->position=%s\n",v->position.print());

    // inform the next and prev that a new view has been added
    // get the index of the next and previous view
    int next = NEXT(next_fill);
    int prev = PREV(next_fill);

    // set unfull's next_fill to the new view
    unfull->neighbors[next_fill] = v;

    // always set one of the newly created view's neighbors to unfull
    int self = INV(next_fill);
    v->neighbors[self] = unfull;

    if (unfull->neighbors[next] != NULL) {
        // if the "next" neighbor exists then set its previous to the new view
        unfull->neighbors[next]->neighbors[prev] = v;

        int inv = INV(prev);

        // be sure to set the new views inv neighbor too.
        v->neighbors[inv] = unfull->neighbors[next];
        debug.info(Debug::Subsystem::Internal, "\tv->neighbor[%i]=%p\n", inv, unfull->neighbors[next]);
    }

    if (unfull->neighbors[prev] != NULL) {
        // if the "prev" neighbor exists then set its next to the new view
        unfull->neighbors[prev]->neighbors[next] = v;

        int inv = INV(next);

        // be sure to set the new views inv neighbor too.
        v->neighbors[inv] = unfull->neighbors[prev];
        debug.info(Debug::Subsystem::Internal, "\tv->neighbor[%i]=%p\n", inv, unfull->neighbors[next]);
    }

    // if the focus is full now, set unfull's 0th to the new unfull
    bool full = true;
    for (int i = 0; i < SIDES; i++) {
        if (unfull->neighbors[i] == NULL) {
            // debug.info("unfull (%p) is not full\n",unfull);
            full = false;
            next_fill = i;  // this is the next place to fill
            break;
        }
    }
    if (full == true) {
        // debug.info("UNFULL IS FULL!\n");
        int new_index = getNextPosition(index);
        unfull = unfull->neighbors[new_index];
        // set the next fill to whatever the next empty spot is on the new unfull
        for (int i = 0; i < SIDES; i++) {
            if (unfull->neighbors[i] == NULL) {
                next_fill = i;
                break;
            }
        }
    }
}

Vector *World::direction(int direction) {
    if (SIDES != 6) {
        // turns out this is not a calculable value
        int degrees = 360 / SIDES;
        debug.info(Debug::Subsystem::Internal, "degrees=%lf\n", degrees);
        double scalar = 1.80278;  // I dunno how to calculate this...
        double x = scalar * COS((direction + 1) * degrees);
        double y = scalar * SIN((direction + 1) * degrees);
        debug.info(Debug::Subsystem::Internal, "x=%lf\n", x);
        debug.info(Debug::Subsystem::Internal, "y=%lf\n", y);
        debug.info(Debug::Subsystem::Internal, "direction=%i\n", direction);
        Vector g(1.5, 1, 0);
        debug.info(Debug::Subsystem::Internal, "g.abs=%lf\n", g.abs());
        return new Vector(x, y, 0);
    } else {
        switch (direction) {
            case 0:  // up right
                return new Vector(1.5, 1, 0);
            case 1:  // up
                return new Vector(0, 2, 0);
            case 2:  // up left
                return new Vector(-1.5, 1, 0);
            case 3:  // down left
                return new Vector(-1.5, -1, 0);
            case 4:  // down
                return new Vector(0, -2, 0);
            case 5:  // down right
                return new Vector(1.5, -1, 0);
        }
    }
    return nullptr;
}

/*     debug.info("macro(%i,%i,%i)\n",a,b,count); */
#define COUNT_MACRO(a, b, count)      \
    {                                 \
        int some_num = a;             \
        while (some_num > 0) {        \
            if (count == 0) return b; \
            count--;                  \
            some_num--;               \
        }                             \
    }

int World::getNextPosition(int view_num) {
    // view_num is zero based.
    if (view_num == 0)
        return 0;
    else {
        // what's the ring num?
        // debug.info("    view_num=%i\n",view_num);

        int ring = 0;
        int sum = SIDES;
        // don't account for the -1 view_num,
        // once we've overshot then we've got
        // the right number of rings
        for (ring = 1; sum < view_num; ring++) sum += ((ring + 1) * SIDES);

        // sum is the number of tiles under this ring and ALL of the tiles on
        // THIS ring.
        // debug.info("    sum=%i\n",sum);

        // put out the ring #
        // ring <= Sumation(SIDES*n + SIDES*(n-1)...);
        // debug.info("    ring=%i\n",ring);

        // the intra_ring_offset is the offset from the beginning of the current
        // ring. so if we are in the 2nd ring at 12 (which starts at 7, zero based)
        // we substract 1 to account for the root tile then subtract the number of
        // tiles in all rings under this one which is ring_count
        int ring_count = 0;
        for (int i = 1; i < ring; i++) ring_count += (i * SIDES);

        int intra_ring_offset = (view_num - 1) - ring_count;
        // debug.info("    intra_ring_offset=%i\n",intra_ring_offset);

        // on ring 1 the pattern is [2,3,4,5,...0,0]
        // on ring 2 the pattern is [1,2,2,3,3,4,4,5,...0,0,0]
        // 1(n)=repetition count. So 1 is repeated n times in the series...
        // on ring n the pattern is [1(n-1), 2(n), 3(n), 4(n), 5(n), ... 0(n+1)]
        for (int j = 1; j <= SIDES; j++) {
            if (j == 1) {
                COUNT_MACRO(ring - 1, j, intra_ring_offset);
            } else if (j == SIDES) {
                COUNT_MACRO(ring + 1, 0, intra_ring_offset);
            } else {
                COUNT_MACRO(ring, j, intra_ring_offset);
            }
        }
    }
    debug.info(Debug::Subsystem::Info, "Something is REALLY screwed up!\n");
    return 0;
}

// adds a light to the view with default parameters
void World::addLight(Vector p) {
    // we can't have more that 8 lights
    if (lights.size() > GL_MAX_LIGHTS) return;

    // instatiate the position of the light to be the current camera position.
    Light *l = new Light(p, SIMPLE, lights.size());

    // set the default colors
    l->setColorAttributes(light_ambient, light_diffuse, light_specular);

    // add the element
    addLight(l);
}

// adds a preconfigured Light
void World::addLight(Light *l) {
    // add the element
    lights.push_back(l);
}

// Turns on or off lights.
void World::shine(bool on) {
    std::list<Light *>::iterator lit = lights.begin();
    while (lit != lights.end()) {
        (*lit)->shine(on);
        lit++;
    }
}

// Creates a spot light based on the camera's view
void World::addSpotLight() {
    Camera *c = &platform->camera;

    // we can't have more that 8 lights
    if (lights.size() > GL_MAX_LIGHTS) return;

    // instatiate the position of the light to be the current camera position.
    Light *l = new Light(c->from, SPOT, lights.size());

    // set the spot light direction ( a-b creates vector from b to a)
    l->direction = c->to - c->from;

    // st the cutoff to the fov
    l->setCutoff((int)c->fov);

    // set the default colors
    l->setColorAttributes(light_ambient, light_diffuse, light_specular);

    // add the element
    addLight(l);
}
