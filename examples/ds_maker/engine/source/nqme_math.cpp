#include "nqme_math.hpp"

int seed;

int randInt()
{
  seed = (1103515245 * seed + 12345) & 0x7fffffff; 
  return seed;
  /*
  seed *= 3;
  seed &= 0xF34B9994;
  seed += 3;
  seed *= seed;
  printf("[RANDOM] %d\n", seed);
  return seed; 
  */
}

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

int min(int a, int b)
{
  if(a < b)
    return a;
  return b;
}
