#include <nds.h>
#include "nqme_engine.hpp"

#define SPRITE_POOL_SIZE 25

#define MAIN_MENU 0
#define GAMEPLAY  1

int DistForKey(
  u8 channel,
  NQME::EventHandler &handler,
  int grace);

int PtsForDist(int dist);
