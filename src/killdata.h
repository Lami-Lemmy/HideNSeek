#pragma once
#include <patch.hh>
#include <rvl/types.h>

extern Instruction<1> Patch_InitKillDataHook;
extern Instruction<1> Patch_NoKillDataHide;
extern "C" {
    extern s32 MenuDataGetPlayerName;
    extern s32 ScreenHUD;
    extern s32 SetMSGID;
}