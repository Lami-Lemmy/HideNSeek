// Sound.cpp - Patches involving sound files

#include <asm.h>
#include <patch.hh>
#include "Sound.hh"

// Auto BRSAR Patching from Elias_
// clang-format off
ASM_FUNCTION(void AutoBRSAR(),
    lis R31, 0x7FFF;
    ori R31, R31, 0xFFFF;
    blr;
)
// clang-format on

void soundInit()
{
    Patch_AutoBRSARHook.setBL(AutoBRSAR);
}