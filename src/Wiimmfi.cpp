// Wiimmfi.cpp - Region patches for online
/*
    This file was originally reverse engineered from code in LE-Code that was written by Leseratte/Wiimm.
    It's also went through several versions across different mods and patching systems, so contains some
    edits from Seeky, TheLordScruffy, and CLF78 at various points.
*/
#include <asm.h>
#include <patch.hh>
#include <string.h>
#include "main.h"
#include "Wiimmfi.hh"

u32 expectedHash[] = {0x0FFF1F07, 0x00E638C9, 0x49FBEFFA, 0x79022D3A, 0x84AB134F};

static void onlineregionInit()
{
    // Patch login region
    // 120053 in ASCII
    strcpy(Patch_LoginRegion, "120053");
    Patch_VSRegion1.setWord(0x38A04E55);
    Patch_VSRegion2.setWord(0x38A04E55);
    Patch_VSRegion3.setWord(0x38A04E55);
    Patch_VSRegion4.setWord(0x38800055);
}

static void patchURL(u32 offset, const char *string)
{
    strcpy(&Patch_WiimmfiURL[offset], string);
}

// clang-format off
ASM_FUNCTION(extern "C" void wiimmfiAsm1(),
    // Original instruction
    cmpwi R3, 0;
    
    // Return workaround
    mflr R23;

    ble w1_end;

    // r13 replacements
    lis R11, DWC_AuthServer@ha;
    lis R12, SSL_Initialised@ha;

    lwz R3, 0xC(1);
    lwz R0, DWC_AuthServer@l (11);
    cmpwi R0, 2;
    beq w1_cont;

    stw R3, SSL_Initialised@l (R12);
    li R0, 2;
    stw R0, DWC_AuthServer@l (R11);
    b w1_end;

    // Execute payload
    w1_cont:
    addi R4, R3, R3;
    rlwinm R4, R4, 0, 0, 29;
    lbz R5, 0x0(R4);
    add R5, R4, R5;
    dcbf 0, R5;
    mtlr R5;
    blrl;

    // Original instruction
    w1_end:
    li R3, -1;
    cmpwi R3, 0;

    // Return workaround
    mtlr R23;
    li R23, 0;
    blr;
);

ASM_FUNCTION(extern "C" void wiimmfiAsm2(),
    // Return workaround 
	stwu R1, -8 (R1);
	mflr R3;
	stw R3, 4 (R1);

    lis R12, SSL_Initialised@ha;

    // Check if inited
    lwz R4, SSL_Initialised@l (R12);
    cmplwi R4, 1;
    ble w2_nomatch;

    // Push stack
    stwu R1, -0x80(R1);

    // Call NETSHA1Init
    addi R3, R1, 0x20;
    bl NETSHA1Init;

    // Call NETSHA1Update
    addi R3, R1, 0x20;
    lis R12, SSL_Initialised@ha;
    lwz R4, SSL_Initialised@l (12);
    li R5, 0x554;
    stw R5, 0xC4(28);
    bl NETSHA1Update;

    // Call NETSHA1GetDigest
    addi R3, R1, 0x20;
    addi R4, R1, 0x10;
    bl NETSHA1GetDigest;

    // Setup loop
    lis R3, (expectedHash-4)@h;
    ori R3, R3, (expectedHash-4)@l;
    addi R4, R1, 0xC;
    li R5, 5;
    mtctr R5;

    // Loop it!
    w2_loop:
    lwzu R5, 0x4(R3);
    lwzu R6, 0x4(R4);
    cmpw R6, R5;
    bne w2_exit;
    bdnz+ w2_loop;

    // Check if we found a match and pop the stack
    w2_exit:
    cmpw R6, R5;
    addi R1, R1, 0x80;
    lis R12, SSL_Initialised@ha;
    lwz R4, SSL_Initialised@l (R12);
    beq w2_end;

    // Return 0 otherwise
    w2_nomatch:
    li R4, 0;

    w2_end:
    // Return workaround
	lwz R3, 4 (R1);
	mtlr R3;
	addi R1, R1, 8;
	blr;
);

void wiimmfiPatcher()
{
    Patch_DWC_Authserver_Debug.setWord(0x3bc00000); // Force DWC_AUTHSERVER_DEBUG
    #ifdef DEBUG
    Patch_DWC_Logmask.setWord(0xFFFFFFFF);          // Force DWC_LOGMASK
    #endif
    strcpy(Patch_WiimmfiVersion, "LE-CODE GCT v1"); // Patcher name to please leseratte
    patchURL(0x0, "://ca.nas.wiimmfi.de/ca");
    patchURL(0x28, "://ca.nas.wiimmfi.de/ca");
    patchURL(0xA8, "://naswii.wiimmfi.de/pr");

    switch (*(char *)0x80000003) // Region check
    {
    case 'P': // PAL
        patchURL(0x50, "main.nas.wiimmfi.de/pp");
        break;
    case 'E': // NTSC-U
        patchURL(0x50, "main.nas.wiimmfi.de/pe");
        break;
    case 'J': // NTSC-J
        patchURL(0x50, "main.nas.wiimmfi.de/pj");
        break;
    case 'K': // NTSC-K
        patchURL(0x50, "main.nas.wiimmfi.de/pk");
        break;
    default:
        u32 fg = 0xFFFFFFFF, bg = 0;
        OSFatal(&fg, &bg, "Invalid Region!"); // Should never happen
        break;
    }

    patchURL(0x964, "wiimmfi.de");  // Available
    patchURL(0x10D4, "wiimmfi.de"); // GPCM
    patchURL(0x1AEC, "wiimmfi.de"); // GPSP
    patchURL(0x2C8D, "wiimmfi.de"); // Master
    patchURL(0x38A7, "wiimmfi.de"); // Natneg
    patchURL(0x38C3, "wiimmfi.de");
    patchURL(0x38DF, "wiimmfi.de");
    patchURL(0x3A2F, "wiimmfi.de"); // MS
    patchURL(0x3AB3, "wiimmfi.de"); // SAKE

    Patch_WiimmfiHook1.setBL(wiimmfiAsm1);
    Patch_WiimmfiHook2.setBL(wiimmfiAsm2);
    onlineregionInit();
}