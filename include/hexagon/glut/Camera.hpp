/**
 * COPYRIGHT (C) 2003-2026, ERIK RAINEY.
 *
 * This source code has not yet been approved for release under any license
 * and as such has no warranty or fitness of purpose associated with it. Use at
 * your own discretion.
 *
 * @author Erik Rainey
 * @file Camera.hpp
 */

#ifndef CAMERA_H
#define CAMERA_H

#include "hexagon/Vector.hpp"
#include <cstdint>
#include <numbers>

/**
 * @class Camera
 * @brief 3D camera view and navigation controller.
 */
class Camera {
public:
    enum /**
          * @class Type
          * @brief Description for class Type.
          */
        class Type : uint32_t {
        Free,
        Spherical
    };

    // The basic up camera vector;
    Vector up;
    // Where the camera is
    Vector from;
    // Where the camera is pointing
    Vector to;
    // The movememnt vector, always normalized
    Vector pan;
    // The location where we want to move our focus
    Vector dest;

    int state;

    float aspect;
    float fov;

    constexpr static auto MIN_ZOOM{10.0f};
    constexpr static auto MAX_ZOOM{100.0f};
    constexpr static auto NAMING_DISTANCE{(MAX_ZOOM / 2.0f)};
    float radius;
    float phi;
    constexpr static auto PHI_MIN{0.1f};
    constexpr static auto PHI_MAX{(std::numbers::pi / 2.0f) - PHI_MIN};
    float theta;
    float accel;

    Type mode;

    // class constructor
    Camera();
    // class destructor
    ~Camera();
    // this places the camera in the world (for rendering)
    void place(void);
    // This computes the new location of the camera.
    void move(void);
    // GLUT Callback for doing object selection
    void selection(int x, int y);
    // Re-Initializes the camera.
    void init(void);
    // dumps the information about the camera
    char *printInfo(void);
    // Unfreezes the camera
    void unfreeze(void);
    // Freezes the camera where is it.
    void freeze(void);
    // Sets the new distance
    void setDistance(float d);
    // Alters the distance of the camera.
    void alterDistance(float delta);
};

#endif    // CAMERA_H
