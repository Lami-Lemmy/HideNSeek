#pragma once
#include <rvl/types.h>

#ifdef __cplusplus
extern "C" {
#endif

char *strcpy(char *dest, const char *src);
int strcmp(const char* s1, const char* s2);
size_t strlen(const char* str);
void* memcpy(void* dest, const void* src, size_t len);
void* memset(void* dest, int c, size_t len);
void* memmove(void* dest, const void* src, size_t len);

#ifdef __cplusplus
}
#endif