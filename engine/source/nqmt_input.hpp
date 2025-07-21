#ifndef NQMT_INPUT_H 
#define NQMT_INPUT_H

#include <nds.h>
#include "nqmt_math.hpp"

namespace NQMT
{

void UpdateInputs();

bool isButtonPressed(u16 button);
bool isButtonJustPressed(u16 button);
bool isButtonReleased(u16 button);
bool isButtonJustReleased(u16 button);

Vector2i getTouchPosition();

}

#endif