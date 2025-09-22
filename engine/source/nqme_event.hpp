#pragma once
#ifndef NQME_EVENT_HPP
#define NQME_EVENT_HPP 

#include <nds.h>

namespace NQME
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
  EventHandler();
  EventHandler(const char* path, u8 capacity, event* buf);
  void Init(const char* path, u8 capacity, event* buf);
  FILE* source;
  u16 grace;
  u8 size;
  event* buffer;
  u32 time;
  bool eof = false;
  void Update(u32 date);
  void Close();
  bool Ended();
};

}

#endif
