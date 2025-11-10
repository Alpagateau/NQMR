#ifndef NQME_INPUT_H 
#define NQME_INPUT_H

#include <nds.h>
#include "nqme_math.hpp"

namespace NQME
{

void UpdateInputs();

bool Pressed(u16 button);
bool JustPressed(u16 button);
bool isButtonReleased(u16 button);
bool isButtonJustReleased(u16 button);

Vector2i getTouchPosition();

}

#endif