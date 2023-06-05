// Wiimmfi.h
#pragma once
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
u32 expectedHash[] = {0x0FFF1F07, 0x00E638C9, 0x49FBEFFA, 0x79022D3A, 0x84AB134F};
extern Instruction<1> Patch_VSRegion1;
extern Instruction<1> Patch_VSRegion2;
extern Instruction<1> Patch_VSRegion3;
extern Instruction<1> Patch_VSRegion4;
extern Instruction<1> Patch_WiimmfiHook1;
extern Instruction<1> Patch_WiimmfiHook2;

void onlineregionInit();