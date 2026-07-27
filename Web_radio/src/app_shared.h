#pragma once

#include <stdint.h>

enum PlayerMode : uint8_t
{
  MODE_RADIO = 0,
  MODE_SD_MP3 = 1,
  MODE_JELLYFIN = 2
};

void setPlayerMode(PlayerMode mode);
void playNextInCurrentMode();
void playPrevInCurrentMode();
