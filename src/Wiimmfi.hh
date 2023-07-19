// Wiimmfi.h
#pragma once
#include <patch.hh>
#include <rvl/types.h>
extern "C"
{
    extern const char *Patch_AuthserverHosts[3];
    extern char Patch_LoginRegion[];
    extern char Patch_WiimmfiURL[];
    extern char Patch_WiimmfiVersion[];
    extern s32 DWC_AuthServer, SSL_Initialised;
    void NETSHA1Init();
    void NETSHA1Update();
    void NETSHA1GetDigest();
}

extern Instruction<1> Patch_DWC_Authserver_Debug;
extern Instruction<1> Patch_DWC_Logmask;
extern Instruction<1> Patch_VSRegion1;
extern Instruction<1> Patch_VSRegion2;
extern Instruction<1> Patch_VSRegion3;
extern Instruction<1> Patch_VSRegion4;
extern Instruction<1> Patch_WiimmfiHook1;
extern Instruction<1> Patch_WiimmfiHook2;

void wiimmfiPatcher();