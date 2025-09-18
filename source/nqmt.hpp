#pragma once
#include <nds.h>
#include "nqme_engine.hpp"
#include <vector>

#define SPRITE_POOL_SIZE 20

#define MAIN_MENU 0
#define GAMEPLAY  1

#define SCREEN_GAP 8

#define SAVE_OFFSET 0
struct SaveData 
{
  int newSave;
  int[5] scores;
};

bool loadSave();
bool saveSave();

struct GameData 
{
  char music_name[40];
  int pts = 0;
  bool success = false;
};

int DistForKey(
  u8 channel,
  NQME::EventHandler &handler,
  int grace);

int PtsForDist(int dist);
