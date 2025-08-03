#ifndef NTS_MATH_H
#define NTS_MATH_H

typedef struct {
    float x,y;
} fVec2;


// Distance between p1 and p2
float f2d_magnitude(fVec2 p1, fVec2 p2);

// Normal vector between p1 and p2; 
fVec2 f2d_normal(fVec2 p1, fVec2 p2);

// Add p2 to p1
fVec2 f2d_add(fVec2 p1, fVec2 p2);

#endif