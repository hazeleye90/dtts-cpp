#pragma once
#include <utility>
#include "util.h"
#include <SDL.h>

class DrawVisitor;

class Ship
{
public:
    Ship(CoordXY<double> position);
    Ship(CoordXY<int> position);
    void accept(DrawVisitor *v) const;
    void update();
    void updateHitbox();
    const SDL_Rect &getHitbox() const { return mHitbox; }
    const int getSize() const { return mSize; }
    const double getMaxAccel() const { return mMaxAccel; }
    const CoordXY<double> getPosition() const { return mPosition; }
    void setPosition(CoordXY<double> position)
    {
        mPosition = position;
        mVelocity = {0, 0};
        mAcceleration = {0, 0};
    }
    void setPosition(CoordXY<int> position)
    {
        setPosition(CoordXY<double>(position.x, position.y));
    }
    const CoordXY<double> getVelocity() const { return mVelocity; }
    const CoordXY<double> getAcceleration() const { return mAcceleration; };
    
    static const int cTrailLength = 20;
    CoordXY<int> mTrail[cTrailLength];
    int mTrailIndex = 0;

private:
    CoordXY<double> mPosition{0.0, 0.0};
    SDL_Rect mHitbox{0, 0, 0, 0};
    CoordXY<double> mVelocity{0.0, 0.0};
    CoordXY<double> mAcceleration{0.0, 0.0};
    int mSize{16};
    double mMaxAccel{0.5};
};
