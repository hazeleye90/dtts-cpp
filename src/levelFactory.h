#pragma once
#include <string>
#include <vector>
#include "level.h"

struct LevelInfo
{
    std::string levelName;
    std::string levelPath;
};

class LevelFactory
{
public:
    static bool loadLevel(int index, Level& level);

};