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

Color::Color() { memset((char *)colors, 0, sizeof(colors) * sizeof(float)); }

Color::Color(float r, float g, float b, float a) {
    colors[Red] = r;
    colors[Green] = g;
    colors[Blue] = b;
    colors[Alpha] = a;
}

Color::Color(int seed) {
    srand(seed ^ time(NULL));
    int r = rand() % 100;
    int index = r % (Channels - 1);
    int quo = rand() % r;
    float frand = (float)quo / r;

    debug.info(Debug::Subsystem::Info, "+Color::Color() r=%d, Channels=%d, index=%d, quo=%d, frand=%lf\n", r, Channels, index, quo,
                frand);

    memset((char *)colors, 0, sizeof(colors) * sizeof(float));
    colors[index] = frand;
    print();
}

Color::~Color() { debug.info(Debug::Subsystem::Info, "-Color::Color()\n"); }

void Color::print() {
    debug.info(Debug::Subsystem::Info, "Color={%0.2lf,%0.2lf,%0.2lf,%0.2lf}\n", colors[Red], colors[Green], colors[Blue],
                colors[Alpha]);
}

void Color::blendWith(Color *other) {
    if (other == NULL) return;

    float c1red = colors[Red];
    float c1green = colors[Green];
    float c1blue = colors[Blue];
    float c1alpha = colors[Alpha];

    float c2red = other->getRedFloat();
    float c2green = other->getGreenFloat();
    float c2blue = other->getBlueFloat();
    float c2alpha = other->getAlphaFloat();

    colors[Red] = (fabs(c1red - c2red) / 2) + fmin(c1red, c2red);
    colors[Green] = (fabs(c1green - c2green) / 2) + fmin(c1green, c2green);
    colors[Blue] = (fabs(c1blue - c2blue) / 2) + fmin(c1blue, c2blue);
    colors[Alpha] = (fabs(c1alpha - c2alpha) / 2) + fmin(c1alpha, c2alpha);

    print();
}

void Color::perturb(int seed) {
    srand(seed ^ time(NULL));
    int r = rand() % 100;
    int index = r % (Channels - 1);
    int quo = rand() % r;
    float frand = (float)quo / r;
    colors[index] = frand;
    print();
}

float Color::operator[](int index) {
    if (index < 0)
        index = 0;
    else if (index >= Channels)
        index = Channels - 1;
    return colors[index];
}

void Color::setRedFloat(float value) { setColorIndex(Red, value); }

void Color::setGreenFloat(float value) { setColorIndex(Green, value); }

void Color::setBlueFloat(float value) { setColorIndex(Blue, value); }

void Color::setAlphaFloat(float value) { setColorIndex(Alpha, value); }

void Color::setColorIndex(int index, float value) {
    if (value < 0.0)
        value = 0.0;
    else if (value > 1.0)
        value = 1.0;
    colors[index] = value;
}
