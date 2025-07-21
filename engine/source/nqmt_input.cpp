#include "nqmt_input.hpp"

namespace NQMT
{

void UpdateInputs()
{
    scanKeys();
}

bool isButtonPressed(u16 button)
{
    return (keysHeld() & button);
}

bool isButtonJustPressed(u16 button)
{
    return (keysDown() & button)!=0;
}

bool isButtonReleased(u16 button)
{
    return (keysDown() & button) == 0;
}

bool isButtonJustReleased(u16 button)
{
    return (keysUp() & button)!=0;
}

Vector2i getTouchPosition()
{

    touchPosition data;
    touchRead(&data);

    Vector2i pos;
    pos.x = data.px;
    pos.y = data.py;

    return pos;
}
}