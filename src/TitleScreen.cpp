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

void initTitleScreen()
{
    Patch_TitleScreenInit.setBL(InitGeckoCheck);
    return;
}