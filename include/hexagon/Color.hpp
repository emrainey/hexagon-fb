/**
 * COPYRIGHT (C) 2003-2026, ERIK RAINEY.
 *
 * This source code has not yet been approved for release under any license
 * and as such has no warranty or fitness of purpose associated with it. Use at
 * your own discretion.
 *
 * @author Erik Rainey
 * @file Color.hpp
 */

#ifndef _COLOR_H_
#define _COLOR_H_

/**
 * @class Color
 * @brief Represents an RGBA color.
 */
class Color {
    //************
    // VARIABLES
    //************
public:
    enum EColorIndex {
        Red = 0,
        Green,
        Blue,
        Alpha,
        Channels
    };

private:
    float colors[Channels];

    //*********
    // METHODS
    //*********
public:
    Color();
    explicit Color(float r, float g, float b, float a);
    explicit Color(int seed);
    ~Color();

    [[nodiscard]] inline unsigned char getRedByte() const { return ((unsigned char)colors[Red] * 255); }
    [[nodiscard]] inline unsigned char getGreenByte() const { return ((unsigned char)colors[Green] * 255); }
    [[nodiscard]] inline unsigned char getBlueByte() const { return ((unsigned char)colors[Blue] * 255); }
    [[nodiscard]] inline unsigned char getAlphaByte() const { return ((unsigned char)colors[Alpha] * 255); }
    [[nodiscard]] inline float getRedFloat() const { return colors[Red]; }
    [[nodiscard]] inline float getGreenFloat() const { return colors[Green]; }
    [[nodiscard]] inline float getBlueFloat() const { return colors[Blue]; }
    [[nodiscard]] inline float getAlphaFloat() const { return colors[Alpha]; }

    void setRedFloat(float value);
    void setGreenFloat(float value);
    void setBlueFloat(float value);
    void setAlphaFloat(float value);

    void print();
    [[nodiscard]] float const *getFloatArray() const;
    [[nodiscard]] float operator[](int index) const;

    void blendWith(Color const *other);
    void perturb(int seed);

private:
    void setColorIndex(int index, float value);
};

#endif    // _COLOR_H
