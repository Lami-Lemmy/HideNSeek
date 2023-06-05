#pragma once

enum {
    kPlatform32 = 0,
    kPlatform64 = 1,

    kPlatformCurrent = sizeof(void *) == 8,
};
#define static_assert_32bit(s) static_assert(kPlatformCurrent != kPlatform32 || (s))


#ifdef __cplusplus
// lyt_Pane_Base base;
#define BASE(x) x base
#else
// lyt_Pane_Base;
#define BASE(x) x
#endif

