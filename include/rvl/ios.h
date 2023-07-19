#pragma once
#include <rvl/ipc.h>

enum IOSFlags
{
    IPC_OPEN_NONE = 0,
    IPC_OPEN_READ = (1 << 0),
    IPC_OPEN_WRITE = (1 << 1),
    IPC_OPEN_RW = IPC_OPEN_READ | IPC_OPEN_WRITE,
};

typedef enum
{
    IPC_OK = 0,
    IPC_EACCES = -1,
    IPC_EBUSY = -2,
    IPC_EINVAL = -4,
    IPC_ENOENT = -6,
    IPC_EQUEUEFULL = -8,
    IPC_ENOMEM = -22,
} IPCResult;

typedef s32 IOSCommand;