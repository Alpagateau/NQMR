#pragma once

struct Vector2i
{
    int x;
    int y;

    Vector2i operator+(const Vector2i& other) const;
    Vector2i operator-(const Vector2i& other) const;
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
int min(int a, int b);
