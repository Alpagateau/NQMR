#include "nqme_engine.hpp"

namespace NQME
{

void InitNQME()
{
    InitAudio();
    initFS();
    InitBG();
    InitGfx();
}

}
