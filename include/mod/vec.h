#pragma once
#include <rvl/types.h>
#ifdef __cplusplus
extern "C"
{
#endif
    typedef struct
    {
        float x, y, z;
    } VEC3;

    float PSVECSquareDistance(VEC3 *vec1, VEC3 *vec2);

#ifdef __cplusplus
}
#endif