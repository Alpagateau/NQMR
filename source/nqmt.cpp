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

bool loadSave()
{
  // Grant access to save memory (do this once at startup, but here is safe too)
   sysSetCardOwner(BUS_OWNER_ARM9);
    
  // Attempt to read the data from save memory into our struct
  return cardRead(save_data, SAVE_OFFSET, sizeof(SaveData)); 
}

bool saveSave()
{
  // Grant access to save memory
  sysSetCardOwner(BUS_OWNER_ARM9);
    
  // Write our struct data to the save memory
  return cardWrite(save_data, SAVE_OFFSET, sizeof(SaveData));
}



