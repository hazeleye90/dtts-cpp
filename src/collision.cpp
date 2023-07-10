#include "collision.h"
#include <vector>
bool Collision::collideRect(const SDL_Rect& r1, const SDL_Rect& r2)
{
	bool collision = true;

	if ((r1.x >= (r2.x + r2.w))
		|| (r2.x >= (r1.x + r1.w))
		|| (r1.y >= (r2.y + r2.h))
		|| (r2.y >= (r1.y + r1.h)))
	{
		collision = false;
	}

	return collision;
}
/*
bool Collision::collideMultiRect(const SDL_Rect &r1, const std::vector<SDL_Rect &> &rects)
{
    bool collision = false;
	for (auto& r : rects)
	{
		if (collideRect(r1, r)) 
		{
            collision = true;
            break;
		}
	}
    return collision;
}
*/