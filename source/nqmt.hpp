#pragma once
#include <nds.h>
#include "nqme_engine.hpp"
#include <vector>

#define SPRITE_POOL_SIZE 25

#define MAIN_MENU 0
#define GAMEPLAY  1

#define SCREEN_GAP 8

struct GameplayData 
{
  std::string music_name;
  int pts;
};

int DistForKey(
  u8 channel,
  NQME::EventHandler &handler,
  int grace);

int PtsForDist(int dist);
