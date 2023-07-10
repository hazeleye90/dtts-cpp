#include "ship.h"
#include <cmath>
#include "drawing.h"
#include "controller.h"
#include <algorithm>

Ship::Ship(CoordXY<double> position)
{
    mPosition = position;
    mHitbox = {static_cast<int>(mPosition.x) - mSize / 2, static_cast<int>(mPosition.y) - mSize / 2,
               mSize, mSize};
    std::fill_n(mTrail, 20, CoordXY<int>{static_cast<int>(mPosition.x), static_cast<int>(mPosition.y)});
}

Ship::Ship(CoordXY<int> position) { Ship(CoordXY<double>(position.x, position.y)); }

void Ship::accept(DrawVisitor *v) const { v->drawShip(*this); }

void Ship::update()
{
    CoordXY<double> leftStickState = ControlState::instance()->getLeftStick();
    int leftTriggerState = ControlState::instance()->getLeftTrigger();
    mAcceleration = {0, 0};
    if ((leftTriggerState != 0) && leftTriggerState > -32000)
    {
        double scale = std::max(abs(mVelocity.x), abs(mVelocity.y));
        if (scale > 0.001)
        {
            double ltNormalized = (leftTriggerState + 32768) / 65535.0;
            mAcceleration.x = (-mVelocity.x / scale) * mMaxAccel * ltNormalized;
            mAcceleration.y = (-mVelocity.y / scale) * mMaxAccel * ltNormalized;
        }
    }
    else
    {
        if (abs(leftStickState.x) < 0.2)
        {
            leftStickState.x = 0.0;
        }
        if (abs(leftStickState.y) < 0.2)
        {
            leftStickState.y = 0.0;
        }
        mAcceleration.x = leftStickState.x * mMaxAccel;
        mAcceleration.y = leftStickState.y * mMaxAccel;
    }
    if ((mVelocity.x != 0.0) && (std::signbit(mVelocity.x) != std::signbit(mAcceleration.x)) &&
        (abs(mVelocity.x) < abs(mAcceleration.x)))
    {
        mVelocity.x = 0;
    }
    else
    {
        mVelocity.x += mAcceleration.x;
    }
    if ((mVelocity.y != 0.0) && (std::signbit(mVelocity.y) != std::signbit(mAcceleration.y)) &&
        (abs(mVelocity.y) < abs(mAcceleration.y)))
    {
        mVelocity.y = 0;
    }
    else
    {
        mVelocity.y += mAcceleration.y;
    }
    mPosition.x += mVelocity.x;
    mPosition.y += mVelocity.y;
    mTrail[(mTrailIndex++) % cTrailLength] = {static_cast<int>(mPosition.x),
                                              static_cast<int>(mPosition.y)};
    updateHitbox();
}

void Ship::updateHitbox()
{
    mHitbox.x = static_cast<int>(mPosition.x) - mSize / 2;
    mHitbox.y = static_cast<int>(mPosition.y) - mSize / 2;
}
