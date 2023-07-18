// BattleFixes.cpp - Fixes for battle arenas

#include <asm.h>
#include <mod/hidenseek.h>
#include <mod/jgpt.h>
#include <mod/raceinfo.h>
#include <mod/player.h>
#include <mod/vec.h>
#include <patch.hh>
#include "BattleFixes.hh"

// clang-format off
ASM_FUNCTION(void BattleStartFix(),
    lwz R0, 0xB70(R7); // Original instruction
    // Check if the track is a battle arena
    lwz R4, 0xB68(R7);
    cmpwi R4, 0x20;
    bltlr+;
    // If so, force mission starting points (Battle ones are buggy)
    li R0, 4;
    blr;
)

ASM_FUNCTION(void BattleCCFix(),
    lwz R30, 0xB68(R4); // Get track slot
    cmpwi R30, 0x20; // Check if it's a battle arena
    crmove 2, 0; // Move condition to CR0+EQ
    blr;
)

ASM_FUNCTION(void BattleRespawnFixASM(),
    bne returnOg;
    // Get respawn data from R5 and call our custom function
    lwz R4, 0x34(R5);
    bl BattleRespawnFix;
    b Patch_BattleRespawnFixHook+0x2B0;
    returnOg: // Branch back if checkpoints are present 
    b Patch_BattleRespawnFixHook+4;
)
// clang-format on

#define DELFINOPIERCRC 0x72836BEE

extern  "C" VEC3 *GetPlayerPosition(PlayerPointers **);

extern "C" void BattleRespawnFix(RaceinfoPlayer *player, JGPTSection *respawns)
{

    // Initial data
    s8 respawn = -1;
    float minDistance;
    VEC3 *playerPos = GetPlayerPosition(&PlayerHolder->players[player->id]->playerPointers);
    int respawnCount = respawns->pointCount;
    JGPT ***respawnArray = respawns->pointArray;

    // Start loop
    for (int i = 0; i < respawnCount; i++)
    {

        // On Delfino Pier, ignore respawns with range 0 after 2 minutes of total gameplay
        if (ENPTCRC == DELFINOPIERCRC && player->frameCounter >= 7200 && respawnArray[i][0]->range == 0)
            continue;

        // Compute distance
        float currDistance = PSVECSquareDistance(playerPos, &respawnArray[i][0]->pos);

        // If distance is less than the one previously found, pick this respawn
        if (currDistance < minDistance || i == 0)
        {
            minDistance = currDistance;
            respawn = i;
        }
    }

    // Set the respawn we found as the player's current respawn point
    player->respawn = (u8)respawn;
}

void battleFixesInit()
{
    Patch_BattleStartFixHook.setBL(BattleStartFix);
    Patch_BattleCCFixHook.setBL(BattleCCFix);
    Patch_BattleRespawnFixHook.setB(BattleRespawnFixASM);
}