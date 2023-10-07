// TitleScreen.cpp Title screen patches

#include <asm.h>
#include <patch.hh>
#include "TitleScreen.hh"

// Gecko Check Taken from MKW-SP
// clang-format off
ASM_FUNCTION(void InitGeckoCheck(), // Hook into the title screen init 0x8063adbc
    mr R3, R30;
    lbz R4, 0x348(R30); // Original instruction
    b doCheckGecko; // Call CPP
)
// clang-format on

// Check for the presence of the gecko codehandler. If it is there, call OSFatal to crash the game.
// If it is not there, continue on with the game.
extern "C" void doCheckGecko()
{
    if (*(u8 *)0x800018A8 != 0x94)
    {
        return; // Not present
    }
    u32 fg = 0xFFFFFFFF, bg = 0;
    OSFatal(&fg, &bg, "Gecko Detected!"); // Crash the game!
}


// Main Menu Mod

// clang-format off
ASM_FUNCTION(void MainMenuButtonSetup1(),
// Only keep buttons 2 and 3
cmpwi R27, 0x2;
bge keepButton;

// Hide the others
li R3, 1;
stb R3, 0x80(R29);

// Set all buttons to Y coordinate 0
keepButton:
li R3, 0;
stw R3, 0x20(R29);

// Original instruction
cmpwi R27, 0x0;
blr;
)

ASM_FUNCTION(void MainMenuButtonSetup2(),
addi R27, R27, 0x18;

// Reset text
mr R3, R27;
bl ResetText;

// Skipped instruction
stw R27, 0xCA4(R29);
b Patch_MainMenuSetup+0x70C;
)

ASM_FUNCTION(void MainMenuCrashFix(),
// Check if button exists
cmpwi R0, 4;
bgt buttonExists;

// Set button to 1P Wifi
li R0, 4;

// Original instruction
buttonExists:
rlwinm R0, R0, 2, 0, 29;
blr;
)
// clang-format on

void initTitleScreen()
{
#ifndef DEBUG
    Patch_TitleScreenInit.setBL(InitGeckoCheck);
#endif    
    // Main Menu Patches
    Patch_MainMenuSetup.setBL(MainMenuButtonSetup1); // Hides offline options and moves Wifi and Channel ones
    // Patch_MainMenuSetup.setB(MainMenuButtonSetup2); // Removes text inside 1P Wifi button
    Patch_MainMenuCrashFixHook.setBL(MainMenuCrashFix); // Changes default button selection to prevent crashes)
    // Patch_MainMenuSetup.setSmallWord(0, 0x2B); // Disables the buttons for the hidden options
    /*Patch_MainMenuSetup.setWord(0x480006DC, 0x30);
    Patch_MainMenuSetup.setSmallWord(0x00, 0x128);
    Patch_MainMenuSetup.setWord(0x480005E0, 0x12C);
    Patch_MainMenuSetup.setSmallWord(1, 0x413); // Diables 2P Wifi button 
    Patch_MainMenuSetup.setSmallWord(0x18, 0x48F); // Expands 1P Wifi button to fill the empty space
    Patch_MainMenuSetup.setSmallWord(0xA4, 0x4F7); // Fixes THP cropping */
    Patch_MKChannelRedirect.setSmallWord(0x3B); // Redirects MK Channel to credits part 2*/
    return;
}