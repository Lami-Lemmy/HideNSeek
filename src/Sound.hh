#pragma once
#include <patch.hh>

extern Instruction<1> Patch_AutoBRSARHook;
extern Instruction<1> Patch_FastMusicHook;
extern "C" {
    extern s32 MusicPitch; 
    extern s32 MusicState;
    extern s32 SomeJingleThing;
}
void soundInit();