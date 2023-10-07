#pragma once
#include <patch.hh>
extern Instruction<1> Patch_MainMenuCrashFixHook;
extern Instruction<1> Patch_MainMenuSetup;
extern Instruction<1> Patch_MKChannelRedirect;
extern Instruction<1> Patch_TitleScreenInit;

extern "C" {
    extern s32 ResetText;
}