/*******************************************************************************
 * COPYRIGHT (C) 2003-2026, ERIK RAINEY.
 *
 * This source code has not yet been approved for release under any license
 * and as such has no warranty or fitness of purpose associated with it. Use at
 * your own discretion.
 *
 * @author Erik Rainey
 ******************************************************************************/

#include <time.h>
#include <chrono>
#include <thread>
#include <cmath>
#include <cstdlib>

#include "hexagon/Hexagon.hpp"

// define a pointer the Port object
extern Port *platform;

// class constructor
Scheduler::Scheduler() {
    // insert your code here
    debug.info(Debug::Subsystem::Trace, "Scheduler()\n");
}

// class destructor
Scheduler::~Scheduler() {
    // insert your code here
}

// GLUT Callback for the Timed Scheduler
void Scheduler::timed(int value) {
    static int counter = 0;

    if (platform == NULL) return;

    // debug.info(Debug::Subsystem::Trace,"+Scheduler::timer()\n");

    // if we are in the new selected state, then set the selected object's
    // render call item index to selected.

    // else if context menu was selected throw up the thing for that

    // move the camera
    platform->camera.move();

    // move all the objects
    world->focus->top->move();

    // Smoothly interpolate background color towards target_bg
    // Timer fires every 5ms; t = 0.002f interpolates over several seconds
    float t = 0.002f;
    float r = platform->display.bg.getRedFloat();
    float g = platform->display.bg.getGreenFloat();
    float b = platform->display.bg.getBlueFloat();

    float tr = platform->display.target_bg.getRedFloat();
    float tg = platform->display.target_bg.getGreenFloat();
    float tb = platform->display.target_bg.getBlueFloat();

    r += (tr - r) * t;
    g += (tg - g) * t;
    b += (tb - b) * t;

    platform->display.bg.setRedFloat(r);
    platform->display.bg.setGreenFloat(g);
    platform->display.bg.setBlueFloat(b);

    // If close to target color, select a new random dark target color
    float diff = std::abs(r - tr) + std::abs(g - tg) + std::abs(b - tb);
    if (diff < 0.02f) {
        // Keep target components between 0.01 and 0.15 for high contrast dark bg
        float new_r = 0.01f + (float)(std::rand() % 100) / 100.0f * 0.14f;
        float new_g = 0.01f + (float)(std::rand() % 100) / 100.0f * 0.14f;
        float new_b = 0.01f + (float)(std::rand() % 100) / 100.0f * 0.14f;
        platform->display.target_bg = Color(new_r, new_g, new_b, 1.0f);
    }

    // debug.info(Debug::Subsystem::Trace,"-Scheduler::timer()\n");
}

// GLUT Default Schedulerr Callback
void Scheduler::idle(void) {
    if (platform == NULL) return;

    static auto last_time = std::chrono::steady_clock::now();
    auto now = std::chrono::steady_clock::now();
    [[maybe_unused]] auto diff = std::chrono::duration_cast<std::chrono::microseconds>(now - last_time).count();
    last_time = now;

    /* give up the time slice */
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    glutPostRedisplay();
}
