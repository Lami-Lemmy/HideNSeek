#pragma once
#include <rvl/types.h>
#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct
	{
		u8 isStopped, isSeeker, isRealSeeker, unused;
	} HideNSeekPlayer;

	typedef struct
	{
		u8 playerCount, totalSurvivors, totalSeekers, isInfection;
		HideNSeekPlayer players[12];
	} _HideNSeekData;

	extern _HideNSeekData HideNSeekData;
	extern char SeekerCount, PrevSeekers[2], IsInfection, HalfTimer, GlitchesAllowed, BtGlitch, AlwaysWinVote, AlwaysSeeker, SpectatorMode, CurrentSpectated, AmIDead, Have30SecondsPassed, UseReplayCams, MusicState;
	extern u32 ENPTCRC;

#ifdef __cplusplus
}
#endif