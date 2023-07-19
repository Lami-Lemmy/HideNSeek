
#include <asm.h>
#include <rvl/types.h>
#include <mod/killdata.h>
#include <mod/screentext.h>
#include <mod/racedata.h>
#include <string.h>
#include "killdata.h"

void AddNewKillData(u16 type, u16 int1, u16 int2)
{

    // Move the first two entries
    memmove(&KillData.entries[0], &KillData.entries[1], 16);

    // Apply data
    KillData.entries[2].msgId = type;
    KillData.entries[2].int1 = int1;
    KillData.entries[2].int2 = int2;
    KillData.entries[2].timer = 300;

    // Mark as updated
    KillData.needsUpdate = true;
}

void UpdateKillDataText()
{
    screenText textData;

    // Construct screenText
    screenText_construct(&textData);

    // Process each string
    for (int i = 0; i < 3; i++)
    {
        if (KillData.entries[i].msgId != 0 && KillData.entries[i].timer != 0)
            textData.bmgs[i] = 8450 + (KillData.entries[i].msgId - 1) * 3 + i;
        else
            textData.bmgs[i] = 0xFFFFFFFF;
        if (KillData.entries[i].int1 != 0xFFFF)
            textData.playerPtrs[2 * i] = GetBMGPlayer(KillData.entries[i].int1);
        if (KillData.entries[i].int2 != 0xFFFF)
            textData.playerPtrs[2 * i + 1] = GetBMGPlayer(KillData.entries[i].int2);
    }

    // Call setTextSrc
    ApplyKillData(&textData);
}

void UpdateKillData()
{

    // Update timers
    for (int i = 0; i < 3; i++)
    {
        if (KillData.entries[i].timer != 0)
        {
            KillData.entries[i].timer--;
            if (KillData.entries[i].timer == 0)
                KillData.needsUpdate = true;
        }
    }

    // If any timer goes to 0, update texts
    if (KillData.needsUpdate)
    {
        UpdateKillDataText();
        KillData.needsUpdate = false;
    }
}

// clang-format off
ASM_FUNCTION(void InitKillData(),
    // Show element immediately
    li R4, 0;
    stb R4, 0x80(R3);

    // 16:9 X Position = -120.0f
    lis R4, 0xC2F0;

    // Check if widescreen
    lis R5, SystemManager@ha;
    lwz R5, SystemManager@l(R5);
    lwz R5, 0x58(R5);
    cmpwi R5, 1;
    beq+ widescreen;

    // 4:3 X Position = -30.0f
    lis R4, 0xC1F0;

    // Store position
    widescreen:
    stw R4, 0x1C(R3);

    // Y Position = -160.0f
    lis R4, 0xC320;
    stw R4, 0x20(R3);

    // Alignment = Left
    lwz R3, 0x114(R3);
    lwz R3, 0(R3);
    li R4, 0;
    stb R4, 0x100(R3);

    // Initialize KillData
    // Get KillData
    lis R3, KillData@h;
    ori R3, R3, KillData@l+4;

    // Does not require an update
    li R4, 0;
    stb R4, 0x4(R3);

    // Setup loop
    li R4, 0;
    li R5, -1;
    li R6, 3;
    mtctr R6;

    // Loop
    loop:
    stw R4, 0(R3);
    stw R5, 0x4(R3);
    addi R3, R3, 8;
    bdnz+ loop;
    
    // Original instruction
    addi R3, R1, 0x28;
    blr;
);
// clang-format on

ASM_FUNCTION(extern "C" void *GetBMGPlayer(u16 pid),
    mr R4, R3;
    lis R3, MenuData@ha;
    lwz R3, MenuData@l(R3);
    lwz R3, 0x98(R3);
    addi R3, R3, 0x188;
    b MenuDataGetPlayerName;
)

ASM_FUNCTION(extern "C" void ApplyKillData(screenText *textData),
    mr R5, R3;
    lis R3, ScreenHUD@ha;
    lwz R3, ScreenHUD@l(R3);
    lwz R3, 0x5C(R3);
    li R4, 0x45F;
    b SetMSGID;
)
void killDataInit()
{
    Patch_InitKillDataHook.setBL(InitKillData);
    Patch_NoKillDataHide.setHalfWord(0x4800);
}