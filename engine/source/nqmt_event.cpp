#include "nqmt_event.hpp"

namespace NQMT 
{

//template class CircularBuffer<event>;

EventHandler::EventHandler(
  const char *path, 
  u8 capacity, 
  event* buf
)
{
  source = fopen(path, "rb");
  if(source != NULL){ 
    time = 0;
    buffer = buf;
    size = capacity;
  }
}

void EventHandler::Update(u32 date)
{
  time = date;
  for(int i = 0; i < size; i++)
  {
    if(buffer[i].channel != 0)
    {
      int d = time - (int)(buffer[i].time_start);
      if(d > (int)(buffer[i].duration)+grace)
      {
        buffer[i].channel = 0;
      }
    }
  }
  for(int i = 0; i < size; i++)
  {
    if(buffer[i].channel == 0)
    {
      if (fread(&buffer[i], sizeof(event), 1, source) != 1)
      {
        buffer[i] = (event){0};
        printf("Event stream end\n");
        if (feof(source)) break; // End of file
        else perror("File read error");
      }

      if(fread( (void*)&(buffer[i]), sizeof(event), 1, source ) == 0)
      {
        buffer[i] = {0};
      }
    }
  }
}

}
