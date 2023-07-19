#pragma once
#include <rvl/types.h>
#include <mod/player.h>
#include <mod/raceinfo.h>

int UtilRandint(int lo, int hi);
void UtilRandomSeed(int seed);
u32 CalcCRC32(void* data, u32 length);

void DisconnectPlayer(_Raceinfo* raceinfo, int pid);
extern "C" void StarFunc(PlayerSub10* player);

void CustomJingleFunc(int sfx);
void TagFunc(void* something, int pid);

void UpdateTag(int pid, int team);
