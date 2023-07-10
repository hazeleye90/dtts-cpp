#include "levelScene.h"
#include "level.h"
#include "levelFactory.h"
#include "util.h"

LevelScene::LevelScene(const int levelIndex)
{ 
    if (LevelFactory::loadLevel(levelIndex, mLevel))
        mLevel.init();
}

void LevelScene::update() 
{
    mLevel.update();
}

void LevelScene::render() { mLevel.render(); }
