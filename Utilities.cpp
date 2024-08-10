#include "Utilities.h"
#include <math.h>

struct Point 
{
    int x = 0;
    int y = 0;
};	


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
