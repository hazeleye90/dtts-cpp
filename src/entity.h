#pragma once
#include <utility>
#include <SDL.h>
#include "util.h"
#include "constants.h"
#include <vector>
#include <memory>
class DrawVisitor;
class Level;

class Entity
{
public:
    Entity(CoordXY<int> position) : mPosition(position) {}
    virtual void accept(DrawVisitor* v) const = 0;
    virtual void update() = 0;
    virtual void reset() = 0;
    virtual bool hitboxEnabled() = 0;
    virtual const bool isWall() const = 0;
    virtual const bool isDoor() const = 0;
    virtual const bool isSwitch() const = 0;
    virtual const bool isMover() const = 0;
    const SDL_Rect &getHitbox() const { return mHitbox; }
    const CoordXY<int> getPosition() const { return mPosition; }
    void setPosition(CoordXY<int> position) { mPosition = position; }

protected:
    CoordXY<int> mPosition;
    SDL_Rect mHitbox{0, 0, 0, 0};
};

class Wall : public Entity
{
public:
    enum
    {
        UP = 0b00000001,
        RIGHT = 0b00000010,
        DOWN = 0b00000100,
        LEFT = 0b00001000,
        UP_LEFT = 0b00010000,
        UP_RIGHT = 0b00100000,
        DOWN_RIGHT = 0b01000000,
        DOWN_LEFT = 0b10000000,

    };

public:
    Wall(CoordXY<int> position, Level *level) : Entity(position), mLevel(level)
    { mHitbox = {position.x, position.y, cTileSize, cTileSize};
    }
    void accept(DrawVisitor *v) const;
    void update();
    void reset(){}
    bool hitboxEnabled() { return true; }
    const bool isWall() const { return true; }
    const bool isDoor() const { return false; }
    const bool isSwitch() const { return false; }
    const bool isMover() const { return false; }
    void setEdgeFlags(uint8_t flags) { mEdgeFlags = flags; }
    uint8_t getEdgeFlags() const { return mEdgeFlags; }
    const bool isGhost() const;
    const CoordXY<double> getDistanceToShip() const;

private:
    uint8_t mEdgeFlags{0};
    Level *mLevel;
};

class Door : public Entity
{
public:
    Door(CoordXY<int> position, Color color) : Entity(position), mColor(color)
    {
        mHitbox = {position.x, position.y, cTileSize, cTileSize};
    }
    void accept(DrawVisitor *v) const;
    void update();
    void reset() { mIsOpen = (mColor == Color::MAGENTA); }
    bool hitboxEnabled() { return !mIsOpen; }
    const bool isWall() const { return false; }
    const bool isDoor() const { return true; }
    const bool isSwitch() const { return false; }
    const bool isMover() const { return false; }
    const Color getColor() const { return mColor; }
    const bool isOpen() const { return mIsOpen; }
    void setIsOpen(bool isOpen) { mIsOpen = isOpen; }

private:
    bool mIsOpen{false};
    Color mColor;
};

class Switch : public Entity
{
public:
    Switch(CoordXY<int> position, Color color, Level *level);
    void accept(DrawVisitor *v) const;
    void update();
    void reset() { mIsPressed = (mColor == Color::MAGENTA); };
    bool hitboxEnabled() { return !mIsPressed; }
    const bool isWall() const { return false; }
    const bool isDoor() const { return false; }
    const bool isSwitch() const { return true; }
    const bool isMover() const { return false; }
    const Color getColor() const { return mColor; }
    const bool isPressed() const { return mIsPressed; }
    void setIsPressed(bool isPressed) { mIsPressed = isPressed; }

private:
    bool mIsPressed{false};
    Color mColor;
    Level *mLevel;
};

class Mover : public Entity
{
public:
    Mover(CoordXY<int> position, int width, int height, Color color, int direction, Level *level);
    void accept(DrawVisitor *v) const;
    void update();
    void reset();
    bool hitboxEnabled() { return true; }
    const bool isWall() const { return false; }
    const bool isDoor() const { return false; }
    const bool isSwitch() const { return false; }
    const bool isMover() const { return true; }
    const Color getColor() const { return mColor; }
    const int getWidth() const { return mWidth; }
    const int getHeight() const { return mHeight; }
    void initCollide();
    void collide();

private:
    CoordXY<int> mStartPosition;
    int mStartDirection;
    Color mColor;
    int mDirection;
    int mPrevDirection;
    bool mIsVertical;
    int mWidth;
    int mHeight;
    SDL_Rect mLeadBox;
    SDL_Rect mTrailBox;
    std::vector<std::shared_ptr<Entity>> mColliderList;
    Level *mLevel;
};