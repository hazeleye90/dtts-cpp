#pragma once
#include <SDL.h>
#include <vector>

class Collision
{
public:
    static bool collideRect(const SDL_Rect &r1, const SDL_Rect &r2);
    static bool collideMultiRect(const SDL_Rect &r1, const std::vector<SDL_Rect &> &rects);
};