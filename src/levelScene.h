#pragma once
#include "scene.h"
#include "level.h"

class LevelScene : public Scene
{
public:
    LevelScene(const int levelIndex);
    void update();
    void render();
    bool complete() { return mLevel.levelComplete(); }
    void free() { delete this; }

private:
    Level mLevel;
};