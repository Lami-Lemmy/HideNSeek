#pragma once
#include <rvl/ios.h>
#include <rvl/ipc.h>

namespace Dolphin
{
    // Taken from SP
    enum class Ioctlv
    {
        // Merged in df32e3f Nov 10, 2019
        //
        // Dolphin 5.0-11186
        GetElapsedTime = 1, // Vector IN() OUT(u32)
        GetVersion = 2,     // Vector IN() OUT(char[])
        GetSpeedLimit = 3,  // Vector IN() OUT(u32)
        SetSpeedLimit = 4,  // Vector IN(u32) OUT()
        GetCPUSpeed = 5,    // Vector IN() OUT(u32)

        // Merged in 393ce52 May 22, 2020
        //
        // Dolphin 5.0-12058
        GetRealProductCode = 6, // Vector IN() OUT(char[])

        // Merged in 4c2d707 August 7, 2022
        //
        // Dolphin 5.0-17155
        //
        // NOTE: As of 4c2d707/5.0-17155, `DiscordReset` is just a call to `DiscordSetClient`
        // with an empty string.
        DiscordSetClient = 7,   // Vector IN(char[]) OUT()
        DiscordSetPresence = 8, // Vector IN(char[], char[], char[], char[], char[], s64,
                                // s64, u32, u32) OUT()
        DiscordReset = 9,       // Vector IN() OUT()

        // Merged in 083b817 November 12, 2022
        //
        // Dolphin 5.0-17856
        GetSystemTime = 10, // Vector IN() OUT(u64)
    };

} // namespace Dolphin