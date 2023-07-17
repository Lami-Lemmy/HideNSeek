// codes.cpp - General Mod Codes

#include <patch.hh>
#include "codes.hh"
#include "Sound.hh"
#include "Voting.hh"

// Temporary values
u8 tempVal8;
u16 tempVal16;
u32 tempVal32;

// This only contains codes without any branches to custom code
// Those get their own files along with the code itself
void modInit()
{
    // Exception Handler (By Star)
    Patch_ShowExceptions.setWord(0);

    // Battle Close-Up Camera (By _tZ and CLF78)

    Patch_BattleCamera.setSmallWord(1);
    Patch_BattleCamera.setSmallWord(1, 0x494);

    // Diable 5:56 Disconnection (By Ro)
    tempVal16 = 0x4800;
    unsigned int *tempPtr = (unsigned int *)0x8053f478;
    *tempPtr = tempVal16;
    // Patch_No556DC.setHalfWord(tempVal16);

    // Disable Lakitu when going backwards (By CLF78)
    Patch_NoLakitu.setHalfWord(tempVal16);

    // Disable Lap Counting (By Vega, modified be CLF78)
    Patch_NoLapCounting.setHalfWord(tempVal16);

    // Disable Luma (By CLF78)
    Patch_NoLuma.setHalfWord(tempVal16);

    // Disable Title Demo Movie (By Diamond)
    Patch_NoTitleDemo.setHalfWord(tempVal16);

    // Force Teams On (By CLF78)
    Patch_FixResultColors.setWord(0x38000000); // Disables team colors in the final room results
    Patch_ResetSeeker.setWord(0x38000001); // Forces blue team on each player (written later)
    Patch_ResetSeeker.setWord(0x38E00002, 0xB4); // Forces teams mode
    
    // Go To Friends Menu Automatically (By Chadderz)
    Patch_AutoFriendsMenu.setSmallWord(0x8D);
    Patch_AutoFriendsMenu2.setSmallWord(0x28);
    Patch_AutoFriendsMenu3.setSmallWord(0x30);

    // Improved Position Interpolation (By stebler)
    Patch_NoInterpolation.setWord(0x3F800000);

    // License Unlocker (By _tZ)
    Patch_LicenseUnlocker.setWord(0x38600001);

    // No Disconnect (By Bully)
    tempVal32 = 38000000;
    Patch_NoDisconnect1.setWord(tempVal32);
    Patch_NoDisconnect2.setWord(tempVal32);
    Patch_NoDisconnect3.setWord(tempVal32);
    Patch_NoDisconnect4.setWord(tempVal32);

    // Remove Mushroom Bug (By Vega)
    Patch_NoMushroomBug.setSmallWord(0);

    // Set Default Drift Type (By CLF78)
    Patch_DefaultDrift.setWord(0x38600001);

    // Custom code
    soundInit();
    votingInit();

}