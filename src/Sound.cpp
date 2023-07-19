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
);

ASM_FUNCTION(void CustomJingleFunc(int sfx),
    // Check if race is still going
    lis R4, Raceinfo@ha;
    lwz R4, Raceinfo@l(R4);
    lwz R4, 0x28(R4);
    cmpwi R4, 4;
    beqlr;

    // If so, move the sfx if to R4
    mr R4, R3;

    // Instructions copies from the original game
    lis R3, SomeJingleThing@ha;
    lwz R3, SomeJingleThing@l(R3);
    li R5, -1;
    lwz R12, 0(R3);
    lwz R12, 0x20(R12);
    mtctr R12;
    bctr;
);
// clang-format on

// Music Patches (By CLF78)
// clang-format off
ASM_FUNCTION(void FastMusic(),
    mr R30, R3; // Original Instruction
    // Check if music state change is important
    cmpwi R4, 5;
    beq changeState;
    cmpwi R4, 7;
    bnelr+;

    // Reset pitch
    lis R3, 0x3F80;
    lis R4, MusicPitch@ha;
    stw R3, MusicPitch@l(R4);

    // Reset state
    lis R3, 0;
    lis R4, MusicState@ha;
    stw R3, MusicState@l(R4);
    blr;

    // Check if MusicState is already 1
    changeState:
    lis R3, MusicState@ha;
    lbz R4, MusicState@l(R3);
    cmpwi R4, 1;
    bne noPitch;

    // If not, set pitch
    lis R11, 0x3F8C;
    lis R12, MusicPitch@ha;
    stw R11, MusicPitch@l(R12);

    // Add 1 to MusicState
    noPitch:
    addi R4, R4, 1;
    stb R4, MusicState@l(R3);
    blr;
);
// clang-format on

void soundInit()
{
    Patch_AutoBRSARHook.setBL(AutoBRSAR);
    Patch_FastMusicHook.setBL(FastMusic);
}