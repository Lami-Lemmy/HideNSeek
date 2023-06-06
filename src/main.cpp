// main.cpp - General Mod Codes

#include "main.h"
#include <patch.hh>

void codeLoaded()
{
    OSReport("Main Function triggered\n");
    modInit(); // Write gecko codes
    initTitleScreen(); // Gecko Codeloader Checking
    onlineregionInit(); // Online Region Patching
    return;
}

// kmBranch(0x800074d4, codeLoaded);