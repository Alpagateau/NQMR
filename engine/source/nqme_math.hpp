#pragma once

struct Vector2i
{
    int x;
    int y;
};

struct Vector2s
{
    short x;
    short y;
};

struct Vector3i 
{
  int x;
  int y;
  int z;
};

struct Vector3f
{
  float x;
  float y;
  float z;
};

int dot(Vector2i a, Vector2i b);

