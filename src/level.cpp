#include "level.h"
#include "drawing.h"
#include "text.h"
#include "window.h"
#include "collision.h"
#include <algorithm>
#include <iostream>

void Level::init() 
{
    mShip.setPosition(mShipStartPosition);
    mShip.updateHitbox();
    std::fill_n(mShip.mTrail, Ship::cTrailLength, mShipStartPosition);
    for (auto& m : mMovers)
    {
        m->initCollide();
    }
}

void Level::render()
{
    int textWidth = Text::instance()->getStringWidth(mTitle, 6);
    int textPos = (cScreenWidth - textWidth) / 2;
    Text::instance()->setColor(Color::WHITE);
    Text::instance()->placeString(mTitle, textPos, 27, 6);
    for (auto& e : mEntities)
    {
        Drawing::draw(e);
    }
    Drawing::draw(&mShip);
}

void Level::update() 
{
    if (mFrameCount > 15)
    {
        mShip.update();
    }
    for (auto& m : mMovers)
    {
        m->collide();
    }
    for (auto& e : mEntities)
    {
        e->update();
    }
    int collideIndex = checkShipCollision();
    if (collideIndex != -1)
    {
        //if (std::find(mDoors.cbegin(), mDoors.cend(), mEntities[collideIndex]) != mDoors.cend())
        if (mEntities[collideIndex]->isDoor())
        {
            std::cout << "collide with door" << std::endl;
            handleDeath();
            
        }
        //else if (std::find(mWalls.cbegin(), mWalls.cend(), mEntities[collideIndex]) !=
        //    mWalls.cend())
        else if (mEntities[collideIndex]->isWall())
        {
            std::cout << "collide with wall" << std::endl;
            handleDeath();
        }
        //else if (std::find(mMovers.cbegin(), mMovers.cend(), mEntities[collideIndex]) !=
        //    mMovers.cend())
        else if (mEntities[collideIndex]->isMover())
        {
            std::cout << "collide with mover" << std::endl;
            handleDeath();
        }
        //else if (std::find(mSwitches.cbegin(), mSwitches.cend(), mEntities[collideIndex]) !=
        //    mSwitches.cend())
        else if (mEntities[collideIndex]->isSwitch())
        {
            Switch *swi = (Switch *)mEntities[collideIndex].get();
            swi->setIsPressed(true);
            switch (swi->getColor())
            {
            case Color::GREEN:
                for (auto& s : mSwitches)
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
                for (auto& d : mDoors)
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
                for (auto &s : mSwitches)
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
                for (auto &d : mDoors)
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
                for (auto& s : mSwitches)
                {
                    if ((s->getColor() == swi->getColor()) && !s->isPressed())
                    {
                        allPressed = false;
                        break;
                    }
                }
                if (allPressed)
                {
                    for (auto& d : mDoors)
                    {
                        if (d->getColor() == swi->getColor())
                        {
                            d->setIsOpen(true);
                        }
                    }
                }
            }
            std::cout << "collide with switch" << std::endl;
        }
    }
    ++mFrameCount;
}

int Level::checkShipCollision()
{
    int index = -1;
    for (int i = 0; i < mEntities.size(); ++i)
    {
        if (mEntities[i]->hitboxEnabled() && Collision::collideRect(mShip.getHitbox(), mEntities[i]->getHitbox()))
        {
            index = i;
            break;
        }
    }
    return index;
}

void Level::reset()
{
    mFrameCount = 0;
    mShip.setPosition(mShipStartPosition);
    mShip.updateHitbox();
    std::fill_n(mShip.mTrail, Ship::cTrailLength, mShipStartPosition);
    for (auto& e : mEntities)
    {
        e->reset();
    }
}

void Level::handleDeath() 
{ reset(); }

bool Level::levelComplete() { CoordXY shipPos = mShip.getPosition();
    return (shipPos.x < 0) || (shipPos.x > cScreenWidth) || (shipPos.y < 0) ||
           (shipPos.y > cScreenHeight);
}
