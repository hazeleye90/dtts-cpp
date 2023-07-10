#pragma once
#include "entity.h"
#include "ship.h"
#include "util.h"
#include <vector>
#include <string>
#include <memory>


class Level
{
    friend class LevelFactory;
    friend class LevelScene;

public:
    int checkShipCollision();
    void init();
    void render();
    void update();
    void reset();
    void handleDeath();
    bool levelComplete();
    bool isDarkLevel() { return mDarkLevel; }
    Ship *getShip() { return &mShip; }
    const std::vector<std::shared_ptr<Switch>> &getSwitches() { return mSwitches; }
    const std::vector<std::shared_ptr<Door>> &getDoors() { return mDoors; }
    const std::vector<std::shared_ptr<Entity>> &getEntities() const { return mEntities; } 

private:
    std::string mTitle;
    int mIndex{0};
    std::vector<std::shared_ptr<Wall>> mWalls;
    std::vector<std::shared_ptr<Door>> mDoors;
    std::vector<std::shared_ptr<Switch>> mSwitches;
    std::vector<std::shared_ptr<Mover>> mMovers;
    std::vector<std::shared_ptr<Entity>> mEntities;
    bool mDarkLevel{false};
    CoordXY<int> mShipStartPosition{0, 0};
    Ship mShip{CoordXY<double>(0,0)};
    int mFrameCount = 0;
};