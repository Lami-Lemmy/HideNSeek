#pragma once

#include <rvl/types.h>
#include <patch.hh>
extern "C"
{
    extern s32 AlwaysWinVote;
}

extern Instruction<1> Patch_HostAlwaysWinsHook;

void votingInit();