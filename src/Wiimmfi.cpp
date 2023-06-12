// Wiimmfi.cpp - Region patches for online

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
ASM_FUNCTION(extern "C" static void wiimmfiAsm1(),
    // Original instruction
    cmpwi 3, 0;
    
    // Return workaround
    mflr 23;

    ble w1_end;

    // r13 replacements
    lis 11, DWC_AuthServer@ha;
    lis 12, SSL_Initialised@ha;

    lwz 3, 0xC(1);
    lwz 0, DWC_AuthServer@l (11);
    cmpwi 0, 2;
    beq w1_cont;

    stw 3, SSL_Initialised@l (12);
    li 0, 2;
    stw 0, DWC_AuthServer@l (11);
    b w1_end;

    // Execute payload
    w1_cont:
    addi 4, 3, 3;
    rlwinm 4, 4, 0, 0, 29;
    lbz 5, 0x0(4);
    add 5, 4, 5;
    dcbf 0, 5;
    mtlr 5;
    blrl;

    // Original instruction
    w1_end:
    li 3, -1;
    cmpwi 3, 0;

    // Return workaround
    mtlr 23;
    li 23, 0;
    blr;
);

ASM_FUNCTION(extern "C" static void wiimmfiAsm2(),
    // Return workaround 
	stwu 1, -8 (1);
	mflr 3;
	stw 3, 4 (1);

    lis 12, SSL_Initialised@ha;

    // Check if inited
    lwz 4, SSL_Initialised@l (12);
    cmplwi 4, 1;
    ble w2_nomatch;

    // Push stack
    stwu 1, -0x80(1);

    // Call NETSHA1Init
    addi 3, 1, 0x20;
    bl NETSHA1Init;

    // Call NETSHA1Update
    addi 3, 1, 0x20;
    lis 12, SSL_Initialised@ha;
    lwz 4, SSL_Initialised@l (12);
    li 5, 0x554;
    stw 5, 0xC4(28);
    bl NETSHA1Update;

    // Call NETSHA1GetDigest
    addi 3, 1, 0x20;
    addi 4, 1, 0x10;
    bl NETSHA1GetDigest;

    // Setup loop
    lis 3, (expectedHash-4)@h;
    ori 3, 3, (expectedHash-4)@l;
    addi 4, 1, 0xC;
    li 5, 5;
    mtctr 5;

    // Loop it!
    w2_loop:
    lwzu 5, 0x4(3);
    lwzu 6, 0x4(4);
    cmpw 6, 5;
    bne w2_exit;
    bdnz+ w2_loop;

    // Check if we found a match and pop the stack
    w2_exit:
    cmpw 6, 5;
    addi 1, 1, 0x80;
    lis 12, SSL_Initialised@ha;
    lwz 4, SSL_Initialised@l (12);
    beq w2_end;

    // Return 0 otherwise
    w2_nomatch:
    li 4, 0;

    w2_end:
    // Return workaround
	lwz 3, 4 (1);
	mtlr 3;
	addi 1, 1, 8;
	blr;
);

void wiimmfiPatcher()
{
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