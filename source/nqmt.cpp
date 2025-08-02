#include "nqmt.hpp"

int pointsForKey(u8 channel, NQME::EventHandler &eventHandler)
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
  if(min_dist > 0)
  {
    eventHandler.buffer[min_index].channel = 0;
  }
  return min_dist;
}
