#include "entity.h"
#include "drawing.h"
#include "controller.h"
#include "level.h"
#include "collision.h"
#include <iostream>

void Wall::accept(DrawVisitor *v) const { v->drawWall(*this); }

void Wall::update() {}

const bool Wall::isGhost() const { return mLevel->isDarkLevel(); }

const CoordXY<double> Wall::getDistanceToShip() const 
{ 
    CoordXY<double> shipPos = mLevel->getShip()->getPosition();
    return CoordXY<double>(shipPos.x - mPosition.x, shipPos.y - mPosition.y);
}

void Door::accept(DrawVisitor *v) const { v->drawDoor(*this); }

void Door::update() {}

Switch::Switch(CoordXY<int> position, Color color, Level *level)
    : Entity(position), mColor(color), mLevel(level)
{
    mHitbox = {position.x, position.y, cTileSize * 2, cTileSize * 2};
}

void Switch::accept(DrawVisitor *v) const { v->drawSwitch(*this); }

void Switch::update() {}

Mover::Mover(CoordXY<int> position, int width, int height, Color color, int direction, Level *level)
    : Entity(position), mWidth(width), mHeight(height), mColor(color), mDirection(direction),
      mStartPosition(position), mStartDirection(direction), mPrevDirection(direction), mLevel(level)
{
    if (mColor == Color::CYAN)
    {
        mIsVertical = true;
    }
    else
    {
        mIsVertical = false;
    }
    mHitbox = {position.x, position.y, width, height};
    if (mIsVertical)
    {
        mLeadBox = {position.x, position.y + cMoverSpeed, width, height};
        mTrailBox = {position.x, position.y - cMoverSpeed, width, height};
    }
    else
    {
        mLeadBox = {position.x + cMoverSpeed, position.y, width, height};
        mTrailBox = {position.x - cMoverSpeed, position.y, width, height};
    }
}

void Mover::accept(DrawVisitor *v) const { v->drawMover(*this); }

void Mover::update() 
{
    if (mDirection != 0)
    {
        const int delta = mDirection * cMoverSpeed;
        if (mIsVertical)
        {
            mPosition.y += delta;
            mHitbox.y += delta;
            mLeadBox.y += delta;
            mTrailBox.y += delta;
        }
        else
        {
            mPosition.x += delta;
            mHitbox.x += delta;
            mLeadBox.x += delta;
            mTrailBox.x += delta;
        }
    }
}

void Mover::initCollide()
{
    for (auto& e : mLevel->getEntities())
    {
        if (e->isWall() || e->isDoor())
        {
            bool skip = false;
            if (mIsVertical)
            {
                if (((e->getPosition().x + cTileSize) <= mPosition.x) ||
                    (e->getPosition().x >= (mPosition.x + mWidth)))
                {
                    skip = true;
                }
            }
            else
            {
                if (((e->getPosition().y + cTileSize) <= mPosition.y) ||
                    (e->getPosition().y>= (mPosition.y + mHeight)))
                {
                    skip = true;
                }
            }
            if (!skip)
            {
                mColliderList.push_back(e);
            }
        }
        else if (e->isMover() && (e.get() != this))
        {
            bool skip = false;
            Mover *m = (Mover *)e.get();
            if (m->mIsVertical == mIsVertical)
            {
                if (mIsVertical)
                {
                    if (((m->getPosition().x + m->getWidth()) <= mPosition.x)||(m->getPosition().x >= (mPosition.x + mWidth)))
                    {
                        skip = true;
                    }
                }
                else
                {
                    if (((m->getPosition().y + m->getHeight()) <= mPosition.y) ||
                        (m->getPosition().y >= (mPosition.y + mHeight)))
                    {
                        skip = true;
                    }
                }
            }
            if (!skip)
            {
                mColliderList.push_back(e);
            }
        }
        else if (e->isSwitch())
        {
            bool skip = false;
            if (mIsVertical)
            {
                if (((e->getPosition().x + 2*cTileSize) <= mPosition.x) ||
                    (e->getPosition().x >= (mPosition.x + mWidth)))
                {
                    skip = true;
                }
            }
            else
            {
                if (((e->getPosition().y + 2*cTileSize) <= mPosition.y) ||
                    (e->getPosition().y >= (mPosition.y + mHeight)))
                {
                    skip = true;
                }
            }
            if (!skip)
            {
                mColliderList.push_back(e);
            }
        }
    }
}

void Mover::collide()
{
    bool mainCollide = false, leadCollide = false, trailCollide = false;
    for (auto &e : mColliderList /*mLevel->getEntities()*/)
    {
        if (e->isWall() || (e->isDoor() && e->hitboxEnabled()))
        {
            if (Collision::collideRect(mHitbox, e->getHitbox()))
            {
                mainCollide = true;
            }
            if (Collision::collideRect(mLeadBox, e->getHitbox()))
            {
                leadCollide = true;
            }
            if (Collision::collideRect(mTrailBox, e->getHitbox()))
            {
                trailCollide = true;
            }
        }
        else if (e->isMover() && (e.get() != this))
        {
            if (Collision::collideRect(mLeadBox, e->getHitbox()))
            {
                leadCollide = true;
            }
            if (Collision::collideRect(mTrailBox, e->getHitbox()))
            {
                trailCollide = true;
            }
        }
        else if (e->isSwitch() && e->hitboxEnabled())
        {
            bool swMainCollide = false, swLeadCollide = false, swTrailCollide = false;
            Switch *swi = (Switch *)e.get();
            if (Collision::collideRect(mHitbox, swi->getHitbox()))
            {
                swMainCollide = true;
            }
            if (Collision::collideRect(mLeadBox, swi->getHitbox()))
            {
                swLeadCollide = true;
            }
            if (Collision::collideRect(mTrailBox, swi->getHitbox()))
            {
                swTrailCollide = true;
            }
            bool flip = false;
            if (swMainCollide && swLeadCollide && !swTrailCollide && (mDirection == 1))
            {
                flip = true;
            }
            else if (swMainCollide && !swLeadCollide && swTrailCollide && (mDirection == -1))
            {
                flip = true;
            }
            if (flip)
            {
                // This is more or less copied from level.cpp. It may be better if this
                // were moved into its own function
                swi->setIsPressed(true);
                switch (swi->getColor())
                {
                case Color::GREEN:
                    for (auto &s : mLevel->getSwitches())
                    {
                        if (s->getColor() == Color::GREEN)
                        {
                            s->setIsPressed(true);
                        }
                        else if (s->getColor() == Color::MAGENTA)
                        {
                            s->setIsPressed(false);
                        }
                    }
                    for (auto &d : mLevel->getDoors())
                    {
                        if (d->getColor() == Color::GREEN)
                        {
                            d->setIsOpen(true);
                        }
                        else if (d->getColor() == Color::MAGENTA)
                        {
                            d->setIsOpen(false);
                        }
                    }
                    break;
                case Color::MAGENTA:
                    for (auto &s : mLevel->getSwitches())
                    {
                        if (s->getColor() == Color::MAGENTA)
                        {
                            s->setIsPressed(true);
                        }
                        else if (s->getColor() == Color::GREEN)
                        {
                            s->setIsPressed(false);
                        }
                    }
                    for (auto &d : mLevel->getDoors())
                    {
                        if (d->getColor() == Color::MAGENTA)
                        {
                            d->setIsOpen(true);
                        }
                        else if (d->getColor() == Color::GREEN)
                        {
                            d->setIsOpen(false);
                        }
                    }
                    break;
                case Color::RED:
                case Color::BLUE:
                    bool allPressed = true;
                    for (auto &s : mLevel->getSwitches())
                    {
                        if ((s->getColor() == swi->getColor()) && !s->isPressed())
                        {
                            allPressed = false;
                            break;
                        }
                    }
                    if (allPressed)
                    {
                        for (auto &d : mLevel->getDoors())
                        {
                            if (d->getColor() == swi->getColor())
                            {
                                d->setIsOpen(true);
                            }
                        }
                    }
                }
            }
        }
    }
    if (mainCollide || (leadCollide && trailCollide))
    {
        mDirection = 0;
    }
    else if (leadCollide && !trailCollide)
    {
        mDirection = -1;
    }
    else if (!leadCollide && trailCollide)
    {
        mDirection = 1;
    }
    else
    {
        mDirection = mPrevDirection;
    }

    if (mDirection != 0)
    {
        mPrevDirection = mDirection;
    }
}

void Mover::reset()
{
    mPosition = mStartPosition;
    mDirection = mStartDirection;
    mPrevDirection = mStartDirection;
    mHitbox.x = mPosition.x;
    mHitbox.y = mPosition.y;
    if (mIsVertical)
    {
        mLeadBox = {mPosition.x, mPosition.y + cMoverSpeed, mWidth, mHeight};
        mTrailBox = {mPosition.x, mPosition.y - cMoverSpeed, mWidth, mHeight};
    }
    else
    {
        mLeadBox = {mPosition.x + cMoverSpeed, mPosition.y, mWidth, mHeight};
        mTrailBox = {mPosition.x - cMoverSpeed, mPosition.y, mWidth, mHeight};
    }
}
