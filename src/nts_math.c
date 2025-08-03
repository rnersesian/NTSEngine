#include "nts_math.h"
#include "math.h"

float f2d_magnitude(fVec2 p1, fVec2 p2) {
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    return sqrtf(dx * dx + dy * dy);
}

fVec2 f2d_normal(fVec2 p1, fVec2 p2) {
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    float length = sqrtf(dx * dx + dy * dy);
    
    fVec2 result = {0.0f, 0.0f};

    if (length != 0.0f) {
        result.x = dx / length;
        result.y = dy / length;
    }

    return result;
}

fVec2 f2d_add(fVec2 p1, fVec2 p2) {
    fVec2 result;
    result.x = p1.x + p2.x;
    result.y = p1.y + p2.y;
    return result;
}
