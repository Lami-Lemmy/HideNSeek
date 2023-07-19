#include <rvl/types.h>
#include <mod/hidenseek.h>
#include <mod/player.h>
#include <mod/racedata.h>

void UpdateMutes() {

	// If hider, don't do anything
	if (HideNSeekData.players[Racedata->main.scenarios[0].settings.hudPlayerIds[0]].isSeeker) {

		// Mute characters and engines of every hider
		for (int pid = 0; pid < HideNSeekData.playerCount; pid++) {
			PlayerHolder->players[pid]->sound->soundSub->mute = !(HideNSeekData.players[pid].isSeeker);
			PlayerHolder->players[pid]->pointers.playerSound2->muteEngine = !(HideNSeekData.players[pid].isSeeker);
		}
	}
}