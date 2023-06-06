#pragma once
#include <rvl/types.h>
#ifdef __cplusplus
extern "C"
{
#endif

    void DCFlushRange(void *ptr, u32 size);
    void ICInvalidateRange(void *ptr, u32 size);
    bool OSIsTitleInstalled(u64 titleID);
    void OSSleepTicks(u64 ticks);
    void OSReport(const char *format, ...);
    u64 OSGetTime();
    u32 OSGetTick();
    void OSFatal(u32 *fg, u32 *bg, const char *message);

#ifdef __cplusplus
}
#endif