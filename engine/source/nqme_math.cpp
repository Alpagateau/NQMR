#include "nqme_math.hpp"

int dot(Vector2i a, Vector2i b)
{
    return (a.x * b.x + a.y * b.y);
}

Vector2i Vector2i::operator+(const Vector2i& other) const
{
    return {x + other.x, y + other.y};
}

Vector2i Vector2i::operator-(const Vector2i& other) const
{
    return {x - other.x, y - other.y};
}
