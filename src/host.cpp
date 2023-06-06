// host.cpp - Host platform detection

#include "host.h"
#include <rvl/ios.h>
#include <rvl/os.h>
#include <string.h>

#ifdef HOSTCHECK
extern const char diResNameStr[];

// Trigger detection of the host platform
/*
Return Values:
0: Wii, Riivolution
1: Wii, No Riivolution
2: Wii U, Riivolution
3: Wii U, No Riivolution
4: Wii Mini, Riivolution
5: Wii Mini, No Riivolution
6: Dolphin, Riivolution (somehow)
7: Dolphin, No Riivolution
8: Unknown Platform, Riivolution
9: Unknown Platform, No Riivolution
*/
int hostPlatform()
{
    // Check for Dolphin
    if (isDolphin()){
        if (isRiivolution()){
            return 6;
        }
        else{
            return 7;
        }
    }

    // Check for Wii U
    if (isWiiu()){
        if (isRiivolution()){
            return 2;
        }
        else{
            return 3;
        }
    }

    // Check for Wii Mini
    if (isWiiMini()){
        if (isRiivolution()){
            return 4;
        }
        else{
            return 5;
        }
    }
    // Assume Wii
    if (isRiivolution()){
        return 0;
    }
    else{
        return 1;
    }
}

// Dolphin Detection
bool isDolphin()
{
    // Simple /dev/dolphin check
    s32 fd = IOS_Open("/dev/dolphin", 0);
    if (fd >= 0)
    {
        IOS_Close(fd);
        return true;
    }

    // If /dev/dolphin doesn't exist, try an old bug (Palapeli)
    if ((fd = IOS_Open("/title", 1)) == -106)   
    {
        return true;
    }

    if (fd >= 0)
    {
        IOS_Close(fd);
    }
    return false;
}

// WiiU Detection
bool isWiiu()
{
    if (OSIsTitleInstalled(0x0000000100000200))
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Wii Mini Detection
bool isWiiMini(){
    s32 macaddr = IOS_Open("/title/00000001/00000002/data/macaddr.bin", IPC_OPEN_READ);
    if (macaddr >= 0)
    {
        IOS_Close(macaddr);
        return true;
    }
    else
    {
        IOS_Close(macaddr);
        return false;
    }
}

// Riivolution Detection
bool isRiivolution()
{
    // See if the string "/dev/do" is at 0x803858f0 PAL
    if (strcmp(diResNameStr, "/dev/do") == 0)
        return true;

    return false;
}
#endif