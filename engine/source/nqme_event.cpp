#include "nqme_event.hpp"

namespace NQME 
{

EventHandler::EventHandler()
{
}

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
    fread((void*) buffer, sizeof(event), size, source);
  }
}

bool EventHandler::Ended()
{
  if(!eof)
    return false;
  for(int i = 0; i < size; i++)
  {
    if(buffer[i].channel != 0)
      return false;
  }
  return true;
}

void EventHandler::Init(
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
    fread((void*) buffer, sizeof(event), size, source);
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
      if (eof || fread(&buffer[i], sizeof(event), 1, source) != 1 )
      {
        buffer[i] = (event){0};
        if(!eof)
        {
          printf("Event stream end\n");
        }
        eof = true;
        if (feof(source))
        {
          
          break;
        } // End of file
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
