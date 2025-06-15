#include "nqmt_engine.hpp"

namespace NQMT
{

void InitNQMT()
{
    initAudio();
    initFS();
    InitBG();
    InitGfx();
}

}