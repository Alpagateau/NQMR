#include "nqme_engine.hpp"

namespace NQME
{

void InitNQME()
{
    initAudio();
    initFS();
    InitBG();
    InitGfx();
}

}