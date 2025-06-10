#pragma once

typedef struct 
{
    int x;
    int y;
}Vector2i;

typedef struct 
{
    short x;
    short y;
}Vector2s;

int dot(Vector2i a, Vector2i b);