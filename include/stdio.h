#pragma once
#include <rvl/types.h>

#ifdef __cplusplus
extern "C"
{
#endif

    int printf(const char *format, ...);
    int snprintf(char *str, size_t num, const char *format, ...);
    int sprintf(char *str, const char *format, ...);

#ifdef __cplusplus
}
#endif