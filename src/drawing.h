#pragma once
#include <utility>
#include "entity.h"
#include "ship.h"
#include <memory>

class DrawVisitor
{
public:
    void drawShip(const Ship &ship) const;
    void drawWall(const Wall &wall) const;
    void drawDoor(const Door &door) const;
    void drawSwitch(const Switch &swtch) const;
    void drawMover(const Mover &mover) const;
};

namespace Drawing
{
static void draw(std::shared_ptr<Entity> e) 
{ 
    DrawVisitor v;
    e->accept(&v);
}
static void draw(const Ship* s)
{
    DrawVisitor v;
    s->accept(&v);
}
} // namespace Drawing