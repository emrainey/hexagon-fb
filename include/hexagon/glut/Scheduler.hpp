/**
 * COPYRIGHT (C) 2003-2026, ERIK RAINEY.
 *
 * This source code has not yet been approved for release under any license
 * and as such has no warranty or fitness of purpose associated with it. Use at
 * your own discretion.
 *
 * @author Erik Rainey
 * @file Scheduler.hpp
 */

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <chrono>

namespace hexagon {

/**
 * @class Scheduler
 * @brief Timer and idle frame rate scheduler.
 */
class Scheduler {
public:
    std::chrono::steady_clock::time_point timer;
    std::chrono::steady_clock::time_point start;
    std::chrono::steady_clock::time_point finish;
    std::chrono::steady_clock::time_point start_time;

    /// class constructor
    Scheduler();
    /// class destructor
    ~Scheduler();
    /// GLUT Default Idler Callback
    void idle(void);
    /// GLUT Callback for the Timer
    void timed(int value);
};


} // namespace hexagon

#endif    // SCHEDULER_H
