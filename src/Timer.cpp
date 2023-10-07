#include <asm.h>
#include <mod/hidenseek.h>
#include <mod/killdata.h>
#include <mod/musichandler.h>
#include <mod/player.h>
#include <mod/racedata.h>
#include <mod/raceinfo.h>
#include <mod/utils.h>
#include <patch.hh>
#include <rvl/types.h>
#include <string.h>
#include "Timer.h"

// External timer values
u32 TimerVals[3] = {0x8CA0, 0x4650, 0x34BC0};

// Forward declarations
void UpdateMutes();

void MainTimerUpdate(u32 timer)
{
    TimerManager *tmanager; // This is supposed to be a register variable stored in r29 but C++17 removed support for that...

    // Modified original instruction
    if (timer > 0)
        tmanager->frames = timer - 1;

    // Some things happen before 30 seconds have passed
    if (!Have30SecondsPassed)
    {

        // Play the countdown jingle again
        if (timer % 60 == 0 && timer <= 180)
            CustomJingleFunc(0xD8);

        // If timer is 1, reset it back to 10 minutes
        else if (timer == 2)
        {
            tmanager->frames = TimerVals[HalfTimer];
            Have30SecondsPassed = true;

            // Allow the Seekers to move
            for (int pid = 0; pid < HideNSeekData.playerCount; pid++)
            {
                if (HideNSeekData.players[pid].isRealSeeker)
                    HideNSeekData.players[pid].isStopped = false;
            }

            // Play the GO jingle again!
            CustomJingleFunc(0xD9);
        }
    }

    else
    {

        // If one minute is remaining, play the final lap jingle and enable faster music
        if (timer == 3600 && Raceinfo->raceState != 4)
            JingleFunc(MusicHandler, 5);

        // Play the countdown jingle in the last 10 seconds
        else if (timer % 60 == 0)
        {
            if (timer >= 240 && timer <= 600)
                CustomJingleFunc(0xEB);
            else if (timer > 0 && timer <= 180)
                CustomJingleFunc(0xEC);
        }

        // Run the Star function for each Seeker (unless they already have a Star)
        for (int pid = 0; pid < HideNSeekData.playerCount; pid++)
        {
            if (HideNSeekData.players[pid].isSeeker && PlayerHolder->players[pid]->pointers.playerSub10->starTimer == 0)
                StarFunc(PlayerHolder->players[pid]->pointers.playerSub10);
        }
    }

    // Update KillData
    UpdateKillData();

    // Update Character/Vehicle sounds
    UpdateMutes();
}
extern "C" u32 TimerChecks(_Raceinfo *rinfo)
{

    // Return a different value based on which condition is true
    u32 ret = 0;
    if (rinfo->timerManager->frames == 0)
        ret = 1;
    else if (HideNSeekData.totalSurvivors == 0)
        ret = 2;
    else if (HideNSeekData.totalSeekers == 0)
        ret = 3;

    // If any of the conditions is met, end the race
    if (ret > 0)
    {

        // Disable Spectator Mode
        SpectatorMode = 0;

        // Move KillData text upwards so it doesn't clash with the race ended text
        memmove(&KillData.entries[0], &KillData.entries[1], 16);
        KillData.entries[2].timer = 1;

        // Setup loop
        int survivorpos = 1;
        int seekerpos = 1;
        for (int pid = 0; pid < HideNSeekData.playerCount; pid++)
        {

            // For Seekers, set their position to be the amount of survivors + 1, and their points to the total amount of players caught (+ 2 if all players were caught)
            if (HideNSeekData.players[pid].isRealSeeker)
            {
                rinfo->players[pid]->battleScore = HideNSeekData.playerCount - HideNSeekData.totalSurvivors - SeekerCount + (HideNSeekData.totalSurvivors == 0) * 2;
                rinfo->players[pid]->position = HideNSeekData.totalSurvivors + seekerpos;
                seekerpos++;

                // For all uncaught Hiders, set position to survivorpos and points to 15
            }
            else if (rinfo->players[pid]->position == 0)
            {
                rinfo->players[pid]->position = survivorpos;
                rinfo->players[pid]->battleScore = HideNSeekData.playerCount - SeekerCount + 2;
                survivorpos++;
            }

            rinfo->players[pid]->raceFinishTime->minutes = rinfo->players[pid]->position;
        }
    }

    rinfo->players[Racedata->main.scenarios[0].settings.hudPlayerIds[0]]->endReason = ret + 1;
    return ret;
}

bool TimerFlashFix()
{
    return (Have30SecondsPassed && Raceinfo->timerManager->frames <= 3600);
}
// clang-format off
ASM_FUNCTION(void StartingTime(), // Starting Time Modifier
    // Assume regular start timer
    li R31, 1800;
    // Check if HalfTimer is 2
    cmpwi R11, 2;
    bne+ end2;

    // Set it to 3600 if so
    slwi R31, R31, 1;

    // Original Instruction
    end2:
    stw R31, 0x48(R29);
    blr;
);

ASM_FUNCTION(void StartingTime2(), // Starting Time Modifier 2
    li R12, 30;
    // Check if HalfTimer is 2
    lis R11, HalfTimer@ha;
    lwz R11, HalfTimer@l(R11);
    cmpwi R11, 2;
    bne+ end;

    // Set seconds to 0 and minutes to 1
    mr R12, R31;
    sth R12, 0x14(R3);

    // Modified original instruction
    end:
    stb R12, 0x16(R3);
    blr;
);

ASM_FUNCTION(void TimerEnd(),
    lwz R3, 0x4(R31);

    // Check if countdown has ended
    lwz R4, 0x28(R3);
    cmpwi R4, 2;
    blt noEndRace;

    // Call function
    bl TimerChecks;

    // Check if EndReason is not 0
    cmpwi R3, 0;
    bne endRace;

    // Do not end the race yet
    noEndRace:
    b Patch_No5LimitHook+0x98;

    // End the race
    endRace:
    b Patch_No5LimitHook+0x50;
);

ASM_FUNCTION(void TimerEndManager(),
    lbz R4, 0xB84(R3);
    slwi R4, R4, 2;
    lis R3, Raceinfo@ha;
    lwz R3, Raceinfo@l(R3);
    lwz R3, 0xC(R3);
    lwzx R3, R3, R4;
    lwz R3, 0x44(R3);

    // Return (endReason == 2)
    xori R3, R3, 2;
    cntlzw R3, R3;
    srwi R3, R3, 5;
    blr;
);
// clang-format on
void initTimer()
{
    Patch_No5LimitHook.setB(TimerEnd);
    
    return;
}