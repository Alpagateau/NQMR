#ifndef NQMT_EVENT_HPP
#define NQMT_EVENT_HPP 

#include <nds.h>

namespace NQMT
{

//
// ticks are calculated by physical timers 
// They are 

struct event{
    //event channel : used to select what to do
    u8 channel;
    //time the event starts in ticks. 
    u32 time_start;
    //duration of the event, in ticks.
    u16 duration;
};

class EventStream
{
    EventStream(char *path, u8 capacity);
    FILE* file;
    u8 size;
    event* events;
};

}

#endif