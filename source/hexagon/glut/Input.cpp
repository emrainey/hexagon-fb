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
#define ESC (27)
#define ENTER (13)

#define DEBUG_MODIFIER(op, bit)                      \
    {                                                \
        if (glutGetModifiers() == GLUT_ACTIVE_ALT) { \
            debug->systems op(bit);                  \
        }                                            \
    }

// pull in reference for platform
extern Port *platform;

// class constructor
Input::Input() {
    // insert your code here
    debug->info(Debug::Subsystem::Trace, "Input()\n");
}

// class destructor
Input::~Input() {
    // insert your code here
}

// GLUT Mouse Click Callback
void Input::mouseClick(int button, int state, int x, int y) {
    debug->info(Debug::Subsystem::Trace, "+Input::mouseClick(%i,%i,%i,%i)\n", button, state, x, y);

    // these initializations are only done once.
    static int last_x = x;
    static int last_y = y;
    // this is done on every entry
    bool clicked = false;

    // in-place detector...
    if (state == GLUT_DOWN) {
        // capture the x and y
        last_x = x;
        last_y = y;
        // we don't do anything on downs.
        debug->info(Debug::Subsystem::Trace, "-Input::mouseClick() [1]\n");
        return;
    } else if (state == GLUT_UP) {
        // is this click in-place?
        if (last_x == x && last_y == y) {
            clicked = true;
        }
    }

    // it no longer matter which button was pressed, the only difference is the
    // assumed reaction. left button will invoke the "open" command as normal
    // but a right click will show all the possible actions.

    debug->info(Debug::Subsystem::Input, "Selection State = %i\n", world->selection.state);

    // left or right buttons in-place?
    if (button == GLUT_LEFT_BUTTON && clicked == true) {
        if (world->selection.state == SELECTION_CONTEXT_MENU) {
            debug->info(Debug::Subsystem::Input, "Checking Menu for Hit...\n");
            if (world->menu->choose(x, y) == true) {
                debug->info(Debug::Subsystem::Trace, "-Input::mouseClick() [2]\n");

                return;
            }

            delete world->menu;
        }

        // record the selection point on the screen
        world->selection.state = SELECTION_NEW;

        // save the coordinates...
        world->selection.x = x;
        world->selection.y = y;

    } else if (button == GLUT_RIGHT_BUTTON && clicked == true) {
        /* future capability */
        world->selection.state = SELECTION_CONTEXT_MENU;

        // save the coordinates...
        world->selection.x = x;
        world->selection.y = y;
    }

    // get the selection name of the item selected...
    int name = platform->display.drawSelection(x, y);

    debug->info(Debug::Subsystem::Input, "Chosen Name = 0x%08x\n", name);

    if (world->selection.state == SELECTION_CONTEXT_MENU) {
        // delete the old menu...
        delete world->menu;
        // be sure to set it to null so that it won't be rendered.
        world->menu = NULL;
    }

    // find out who is was and execute something on it...
    world->choose(name);

    // set the new camera destination
    debug->info(Debug::Subsystem::Input, "Focus Position=%s\n", world->focus->position.print());

    platform->camera.dest = world->focus->top->position;

    debug->info(Debug::Subsystem::Trace, "-Input::mouseClick()\n");
}

// GLUT Mouse Drag Callback
void Input::mouseDrag(int x, int y) {
    int dx = x - lastx;
    int dy = y - lasty;
    Camera *c = &platform->camera;

    lastx = x;
    lasty = y;

    if (platform->control_state == MOUSE_DRAG) {
        if (dx > 0)
            c->theta -= dx * (PI / 100);
        else if (dx < 0)
            c->theta += -dx * (PI / 100);

        if (dy > 0) {
            c->phi -= dy * (PI / 200);
            if (c->phi <= PHI_MIN) c->phi = PHI_MIN;
        } else if (dy < 0) {
            c->phi += -dy * (PI / 200);
            if (c->phi >= PHI_MAX) c->phi = PHI_MAX;
        }
    } else if (platform->control_state == MOUSE_ZOOM) {
        c->alterDistance(dy);
    } else {
        lastx = x;
        lasty = y;
    }
}

// GLUT Mouse Movement Callback
void Input::mouseMove(int x, int y) {
    lastx = x;
    lasty = y;
}

// GLUT Keyboard Callback for Normal Keys
void Input::keyboardNormal(unsigned char key, int x, int y) {
    debug->info(Debug::Subsystem::Trace, "Key %c (%i) pressed\n", key, key);
    switch (key) {
        case ESC:
            // DestroyStack();
            exit(0);
            break;

        case ENTER:
            createNewView();
            break;

        case '0':
            DEBUG_MODIFIER(=, 0);
            break;

        case '1':
            DEBUG_MODIFIER(^=, Debug::Subsystem::Selection);
            break;

        case '2':
            DEBUG_MODIFIER(^=, Debug::Subsystem::Menu);
            break;

        case '3':
            DEBUG_MODIFIER(^=, Debug::Subsystem::Models);
            break;

        case '4':
            DEBUG_MODIFIER(^=, Debug::Subsystem::Internal);
            break;

        case '5':
            DEBUG_MODIFIER(^=, Debug::Subsystem::Platform);
            break;

        case '6':
            DEBUG_MODIFIER(^=, Debug::Subsystem::Trace);
            break;

        case '*':
            DEBUG_MODIFIER(|=, Debug::Subsystem::Info);
            break;

        case '-':
            platform->camera.alterDistance(1);
            break;

        case '+':
            platform->camera.alterDistance(-1);
            break;

        case ' ':
            /* reinitialize the camera */
            platform->camera.init();
            break;

        case 'l':
        case 'L':
            // add a light to the scene where the camera is!
            world->addSpotLight();
            break;

        case 'i': {
            float c = platform->display.bg.getRedFloat();
            platform->display.bg.setRedFloat(c - 0.1);
            platform->display.bg.print();
            break;
        }
        case 'I': {
            float c = platform->display.bg.getRedFloat();
            platform->display.bg.setRedFloat(c + 0.1);
            platform->display.bg.print();
            break;
        }
        case 'o': {
            float c = platform->display.bg.getGreenFloat();
            platform->display.bg.setGreenFloat(c - 0.1);
            platform->display.bg.print();
            break;
        }
        case 'O': {
            float c = platform->display.bg.getGreenFloat();
            platform->display.bg.setGreenFloat(c + 0.1);
            platform->display.bg.print();
            break;
        }
        case 'p': {
            float c = platform->display.bg.getBlueFloat();
            platform->display.bg.setBlueFloat(c - 0.1);
            platform->display.bg.print();
            break;
        }
        case 'P': {
            float c = platform->display.bg.getBlueFloat();
            platform->display.bg.setBlueFloat(c + 0.1);
            platform->display.bg.print();
            break;
        }

        default:
            break;
    }
    glutPostRedisplay();
}

// GLUT Keyboard Callback for Special Keys
void Input::keyboardSpecial(int key, int x, int y) {
    debug->info(Debug::Subsystem::Trace, "Special Key %i pressed\n", key);
    switch (key) {
        case GLUT_KEY_F1:
            TOGGLE_BOOL(platform->display.renderHelp);
            break;

        case GLUT_KEY_F2:
            TOGGLE_BOOL(debug->state);
            break;

        case GLUT_KEY_F3:
            if (platform->display.blending)
                glEnable(GL_BLEND);
            else
                glDisable(GL_BLEND);
            break;

        case GLUT_KEY_F4:
            toggleControlState();
            break;

        case GLUT_KEY_F5:
            // delete platform;
            // platform = new Port(0,NULL);
            break;

        case GLUT_KEY_F6:
            toggleDisplayText();
            break;

        case GLUT_KEY_F7:
            glutFullScreen();
            break;

        case GLUT_KEY_F8:
            glutReshapeWindow(platform->display.width / 2, platform->display.height / 2);
            glutPositionWindow(50, 50);
            glutSetCursor(GLUT_CURSOR_INHERIT);
            break;

        case GLUT_KEY_F9:
            TOGGLE_BOOL(platform->display.renderGrid);
            break;

        case GLUT_KEY_PAGE_UP:
            platform->camera.radius += 1;
            if (platform->camera.radius > MAX_ZOOM) platform->camera.radius = MAX_ZOOM;

            break;

        case GLUT_KEY_PAGE_DOWN:
            platform->camera.radius -= 1;
            if (platform->camera.radius < MIN_ZOOM) platform->camera.radius = MIN_ZOOM;
            break;

        case GLUT_KEY_UP:
            world->focus->popLayer();
            break;
        default:
            break;
    }
}

void toggleControlState(void) {
    int ctrl = platform->control_state;
    ctrl = (ctrl + 1) % CONTROL_MOD;
    switch (ctrl) {
        case KEYBOARD_CONTROL:
            sprintf(platform->display.control_message, "Keyboard control");
            break;
        case MOUSE_DRAG:
            sprintf(platform->display.control_message, "Mouse Draggable");
            break;
        case MOUSE_ZOOM:
            sprintf(platform->display.control_message, "Mouse Zooming");
            break;
        default:
            sprintf(platform->display.control_message, "Unknown control state %d", ctrl);
            break;
    }

    if (debug->state) {
        debug->info(Debug::Subsystem::Internal, "toggleControlState() %s\n", platform->display.control_message);
        debug->flush();
    }
    platform->control_state = (Control_State_t)ctrl;
}

void toggleDisplayText(void) { TOGGLE_BOOL(platform->display.renderText); }

void createNewView(void) { world->addView(); }
