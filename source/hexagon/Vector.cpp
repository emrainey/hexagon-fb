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

#include "hexagon/Vector.h"

#include <cmath>
#include <cstdio>
#include <string>

// class constructor
Vector::Vector() { zeroize(); }

// another class constructor
Vector::Vector(float xi, float yi, float zi) {
    x = xi;
    y = yi;
    z = zi;
}

void Vector::zeroize() {
    x = 0;
    y = 0;
    z = 0;
}

float Vector::abs() { return (float)sqrt(x * x + y * y + z * z); }

void Vector::normalize() {
    float tol = 0.0001f;  // we gotta define a tolerance otherwise we'll divide
                          // by zero, and be in a real jam.

    double m = abs();
    if (m <= tol) m = 1;

    x /= m;
    y /= m;
    z /= m;

    if (fabs(x) < tol) x = 0.0f;
    if (fabs(y) < tol) y = 0.0f;
    if (fabs(z) < tol) z = 0.0f;
}

void Vector::reverse() {
    x = -x;
    y = -y;
    z = -z;
}

void Vector::set(double a, double b, double c) {
    x = a;
    y = b;
    z = c;
}

// These overloads modify this object

Vector& Vector::operator+=(Vector u) {
    x += u.x;
    y += u.y;
    z += u.z;

    return *this;
}

Vector& Vector::operator-=(Vector u) {
    x -= u.x;
    y -= u.y;
    z -= u.z;

    return *this;
}

Vector& Vector::operator*=(float s) {
    x *= s;
    y *= s;
    z *= s;

    return *this;
}

Vector& Vector::operator/=(float s) {
    x /= s;
    y /= s;
    z /= s;

    return *this;
}

Vector Vector::operator-(void) { return Vector(-x, -y, -z); }

Vector& Vector::operator=(Vector u) {
    x = u.x;
    y = u.y;
    z = u.z;

    return *this;
}

bool Vector::operator==(Vector a) { return (a.abs() == abs()); }

float Vector::operator[](int i) {
    if (i == 0)
        return x;
    else if (i == 1)
        return y;
    else if (i == 2)
        return z;
    else
        return 0;
}

float Vector::getAngleWith(Vector a) {
    // angle between 2 vectors is the inverse cosine of the dot product over the
    // product of the constituent magnitudes.

    // u * v = |u| |v| cos A
    // A = invcos( (u*v) / (|u| |v|);
    // where A is the angle.

    Vector b(x, y, z);
    float dot = a * b;
    return acos(dot / (a.abs() * b.abs()));
}

float Vector::getAngleX() { return acos(x / abs()); }

float Vector::getAngleY() { return acos(y / abs()); }

float Vector::getAngleZ() { return acos(z / abs()); }

// These overloads return new Objects

Vector operator+(Vector u, Vector v) { return Vector(u.x + v.x, u.y + v.y, u.z + v.z); }

Vector operator-(Vector u, Vector v) { return Vector(u.x - v.x, u.y - v.y, u.z - v.z); }

// dot product
float operator*(Vector u, Vector v) { return (u.x * v.x + u.y * v.y + u.z * v.z); }

// cross product
Vector operator^(Vector u, Vector v) {
    return Vector((u.y * v.z - u.z * v.y), (-u.x * v.z + u.z * v.x), (u.x * v.y - u.y * v.x));
}

// Returns a std::string containing debug info for vector
char* Vector::print(void) {
    static char msg[80];
    memset(msg, 0, sizeof(msg));
    snprintf(msg, sizeof(msg), "(%.3lf,%.3lf,%.3lf)", x, y, z);
    return msg;
}
