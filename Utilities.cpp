#include "Utilities.h"
#include <math.h>

void FloatPoint::normalize()
{
    double magnitude = getSize();
    if (magnitude != 0) {
        x = x / magnitude;
        y = y / magnitude;
    }
}

float FloatPoint::getSize()
{
    return sqrt(pow(x,2) + pow(y,2));
}

float getDistanceBetween(FloatPoint a, FloatPoint b)
{
    return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
}

FloatPoint getMidPointBetween(FloatPoint a, FloatPoint b)
{
    FloatPoint midPoint;
    midPoint.x = (a.x + b.x) / 2;
    midPoint.y = (a.y + b.y) / 2;
    return midPoint;
}
