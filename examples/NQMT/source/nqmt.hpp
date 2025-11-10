#pragma once
#include <nds.h>
#include "nqme_engine.hpp"
#include <vector>

#define SPRITE_POOL_SIZE 20

#define MAIN_MENU 0
#define GAMEPLAY  1

#define SCREEN_GAP 8

#define SAVE_OFFSET 0
#define NEW_SAVE_VAL 3342
struct SaveData 
{
  int scores[5];
};

void loadSave();
void saveSave();

struct GameData 
{
  char music_name[40];
  int pts = 0;
  bool success = false;
  u8 song_idx;
};

int DistForKey(
  u8 channel,
  NQME::EventHandler &handler,
  int grace);

int PtsForDist(int dist);
