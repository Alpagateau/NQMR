#include "nqme_input.hpp"

namespace NQME
{

void UpdateInputs()
{
    scanKeys();
}

bool Pressed(u16 button)
{
    return (keysHeld() & button);
}

bool JustPressed(u16 button)
{
    return (keysDown() & button)!=0;
}

bool Released(u16 button)
{
    return (keysDown() & button) == 0;
}

bool JustReleased(u16 button)
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

    return (Pressed(KEY_TOUCH) ? pos : (Vector2i){-1, -1});
}
}
