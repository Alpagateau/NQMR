#include "nqmt.hpp"

GameData game_data;
SaveData save_data;

int DistForKey(u8 channel, NQME::EventHandler &eventHandler, int grace)
{ 
  u8 min_index = 0;
  int min_dist = -1;
  for(u8 i = 0; i < eventHandler.size; i++)
  {
    if(eventHandler.buffer[i].channel == channel)
    {
      int dist = abs( (int)(eventHandler.time - eventHandler.buffer[i].time_start) );
      if( dist < min_dist || min_dist == -1)
      {
        min_index = i;
        min_dist = dist;
      }
    }
  }
  if(min_dist < grace)
  {
    eventHandler.buffer[min_index].channel = 0;
  }
  else 
    min_dist = -1;
  return min_dist;
}

int PtsForDist(int dist)
{
  if(dist < 0)
    return 0;
  if(dist > 10)
    return 10;
  if(dist > 5)
    return 20;
  if(dist > 2)
    return 30;
  if(dist > 0)
    return 50;
  return 100;
}

void saveSave()
{
  //printf("[NO IMP](%s, %d): Trying to load a save\n", __FILE__, __LINE__);
  FILE* save_file = fopen("fat:/nqmt.sav", "wb+");
  if(save_file == NULL)
  {
    printf("[ERROR] Save Save : Couldn't read the SD Card\n");
    return;
  }
  fwrite(&save_data,sizeof(save_data), 1, save_file); 
  fclose(save_file);
}

void loadSave()
{ 
  FILE* save_file = fopen("fat:/nqmt.sav", "rb");
  if(save_file == NULL)
  {
    printf("[ERROR] Load Save : Couldn't read the SD Card\n");
    return;
  }
  SaveData sd = {0};
  int i = fread(&sd, sizeof(SaveData), 1, save_file);
  if(i <= 0)
  {
    printf("[ERROR] Couldn't read the save file\n");
  }
  save_data = sd;
  fclose(save_file);
}



