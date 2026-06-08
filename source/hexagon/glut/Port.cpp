/*******************************************************************************
 * COPYRIGHT (C) 2003-2026, ERIK RAINEY.
 *
 * This source code has not yet been approved for release under any license
 * and as such has no warranty or fitness of purpose associated with it. Use at
 * your own discretion.
 *
 * @author Erik Rainey
 ******************************************************************************/

#include "hexagon/Hexagon.hpp"
#include "hexagon/Hexagon_Console_private.hpp"

// class constructor
Port::Port(int argc, char *argv[]) {
    static char title[1024];

    scheduler.start_time = std::chrono::steady_clock::now();
    debug.info(Debug::Subsystem::Trace, "+Port()\n");

    snprintf(title, sizeof(title), "%s (%s) by %s. Built at %s %s", PRODUCT_NAME, PRODUCT_VERSION, PROJECT_AUTHOR, __DATE__, __TIME__);

    glutInit(&argc, argv);
    // Create an RGA, Alpha Channel, Double Buffered Multisampled, Depth Buffered
    // rendering ... thingy.
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE | GLUT_MULTISAMPLE | GLUT_ALPHA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(640, 480);
    glutCreateWindow(title);

    // initialize all the member variables
    control_state = MOUSE_DRAG;

    // initialize the persistent storage to a platform specific version!
    ps = new PORT_SPECIFIC_PERSISTENT_STORAGE();

    // initialize the port specific action retriever
    ar = new PORT_SPECIFIC_ACTION_RETRIEVER();

    // initialize the Shell Executor...
    shell = new PORT_SPECIFIC_SHELL_EXECUTOR();

    if (debug.state) {
        Key date_key("Compile Date", __DATE__);
        Key time_key("Compile Time", __TIME__);
        Key version("Version", PRODUCT_VERSION);
        ps->setKey(date_key);
        ps->setKey(time_key);
        ps->setKey(version);
        Key *k1 = ps->getKey("Compile Date");
        Key *k2 = ps->getKey("Compile Time");
        Key *k3 = ps->getKey("Version");
        if (k1 && k2 && k3) {
            debug.info(Debug::Subsystem::Platform, "Key:%s:%s\n", k1->getName().c_str(), k1->getValue().c_str());
            debug.info(Debug::Subsystem::Platform, "Key:%s:%s\n", k2->getName().c_str(), k2->getValue().c_str());
            debug.info(Debug::Subsystem::Platform, "Key:%s:%s\n", k3->getName().c_str(), k3->getValue().c_str());
        } else {
            debug.info(Debug::Subsystem::Platform, "Compiled keys not found in persistent storage.\n");
        }
    }

    // time to hook up all the GLUT stuff!
    glutWrappers_assignCallbacks();

    debug.info(Debug::Subsystem::Trace, "-Port()\n");
}

// class destructor
Port::~Port() {
    // insert your code here
}

void Port::run() {
    display.fps = 60;
    scheduler.start = std::chrono::steady_clock::now();
    glutMainLoop();
}

/*
 * Do any post-instantiation initialization.
 */
void Port::postInitialize() {
    display.postInitialize();
}
