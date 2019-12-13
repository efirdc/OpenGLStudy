#ifndef _SDF
#define _SDF

#include "material.glsl"

struct sdResult
{
    float d;
    Material mat;
};

sdResult sdUnion(sdResult a, sdResult b)
{
    return a.d < b.d ? a : b;
}

sdResult sdInersect(sdResult a, sdResult b)
{
    return a.d > b.d ? a : b;
}

sdResult sdDifference(sdResult a, sdResult b)
{
    return a.d > -b.d ? a : b;
}

sdResult sdSphere(vec3 p, float r, Material mat)
{
  return sdResult(length(p) - r, mat);
}

#endif