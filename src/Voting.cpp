// Voting.cpp - Patches involving track voting

#include <asm.h>
#include <patch.hh>
#include "Voting.hh"

// clang-format off
ASM_FUNCTION(void HostAlwaysWins(), // Always Win Track Vote (By Star, modified by Seeky and CLF78)
    // Check if the option is enabled
    lis R12, AlwaysWinVote@ha;
    lbz R12, AlwaysWinVote@l(R12);
    cmpwi R12, 1;
    bne end;

    // If it is, replace the player ID with the host's
    li R4, 0;

    // Original instruction
    end:
    stb R4, 0x3E(R31);
    blr;
)
// clang-format on

void votingInit()
{
    Patch_HostAlwaysWinsHook.setBL(HostAlwaysWins);
}