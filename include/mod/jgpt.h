#pragma once
#include <rvl/types.h>
#include <mod/vec.h>
#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        VEC3 pos, rot;
        u16 id;
        s16 range;
    } JGPT;

    typedef struct
    {
        JGPT ***pointArray;
        u16 pointCount, unk;
        void *raw;
    } JGPTSection;

#ifdef __cplusplus
}
#endif