// main.cpp - General Mod Codes

#include "main.h"
#include <patch.hh>
void codeLoaded()
{
    OSReport("Main Function triggered\n");
    modInit(); // Write mod codes
    initTitleScreen(); // Gecko Codeloader Checking
    wiimmfiPatcher(); // Wiimmfi Patcher
    return;
}

// kmBranch(0x800074d4, codeLoaded);