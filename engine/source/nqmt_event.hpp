#pragma once
#ifndef NQMT_EVENT_HPP
#define NQMT_EVENT_HPP 

#include <nds.h>

namespace NQMT
{
// ticks are calculated by physical timers 
#pragma pack(push, 1)
struct __attribute__((__packed__)) event {
    //event channel : used to select what to do
    u8 channel;
    //time the event starts in ticks. 
    u32 time_start;
    //duration of the event, in ticks.
    u16 duration;
};
#pragma pack(pop)

class EventHandler
{
public:
  //EventHandler();
  EventHandler(const char* path, u8 capacity, event* buf);
  FILE* source;
  u16 grace;
  u8 size;
  event* buffer;
  u32 time;
  void Update(u32 date);
};

}

#endif
