#include "drawing.h"
#include "window.h"
#include "constants.h"
#include "text.h"
#include "level.h"
#include <cmath>
#include <SDL.h>

const int cThickness = 2;

void DrawVisitor::drawShip(const Ship &ship) const
{
    SDL_Renderer *renderer = Window::instance()->getRenderer();
    const int size = ship.getSize();
    const auto coord = ship.getPosition();
    CoordXY<double> plumeLen = ship.getAcceleration();
    plumeLen.x *= -size;
    plumeLen.y *= -size;
    CoordXY<double> plumePos = coord;
    if (plumeLen.x < 0)
    {
        plumePos.x -= size / 2 - ceil(plumeLen.x);
    }
    else
    {
        plumePos.x += size / 2;
    }
    if (plumeLen.y < 0)
    {
        plumePos.y -= size / 2 - ceil(plumeLen.y);
    }
    else
    {
        plumePos.y += size / 2;
    }
    SDL_Rect plumeRectX = {static_cast<int>(plumePos.x), static_cast<int>(coord.y - size / 4),
                           static_cast<int>(abs(plumeLen.x)), size / 2};
    SDL_Rect plumeRectY = {static_cast<int>(coord.x - size / 4), static_cast<int>(plumePos.y),
                           size / 2, static_cast<int>(abs(plumeLen.y))};
    SDL_Rect rect1 = {static_cast<int>(coord.x - size / 4), static_cast<int>(coord.y - size / 2),
                      size / 2, size};
    SDL_Rect rect2 = {static_cast<int>(coord.x - size / 2), static_cast<int>(coord.y - size / 4),
                      size, size / 2};
    SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0xFF, 0xFF);
    SDL_RenderFillRect(renderer, &plumeRectX);
    SDL_RenderFillRect(renderer, &plumeRectY);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderFillRect(renderer, &rect1);
    SDL_RenderFillRect(renderer, &rect2);
    for (int i = 0; i < Ship::cTrailLength - 1; i++)
    {
        auto startPoint = ship.mTrail[(i + ship.mTrailIndex) % Ship::cTrailLength];
        auto endPoint = ship.mTrail[(i + 1 + ship.mTrailIndex) % Ship::cTrailLength];
        SDL_RenderDrawLine(Window::instance()->getRenderer(), startPoint.x, startPoint.y,
                           endPoint.x, endPoint.y);
    }
}

void DrawVisitor::drawWall(const Wall &wall) const 
{ 
    auto coord = wall.getPosition();
    SDL_SetRenderDrawColor(Window::instance()->getRenderer(), 0xff, 0xff, 0xff, 0xff);
    uint8_t edgeFlags = wall.getEdgeFlags();
    if (!wall.isGhost())
    {
        if (edgeFlags & Wall::UP)
        {
            SDL_Rect upEdge = {coord.x, coord.y, cTileSize, cThickness};
            SDL_RenderFillRect(Window::instance()->getRenderer(), &upEdge);
        }
        if (edgeFlags & Wall::RIGHT)
        {
            SDL_Rect rightEdge = {coord.x + cTileSize - cThickness, coord.y, cThickness, cTileSize};
            SDL_RenderFillRect(Window::instance()->getRenderer(), &rightEdge);
        }
        if (edgeFlags & Wall::DOWN)
        {
            SDL_Rect downEdge = {coord.x, coord.y + cTileSize - cThickness, cTileSize, cThickness};
            SDL_RenderFillRect(Window::instance()->getRenderer(), &downEdge);
        }
        if (edgeFlags & Wall::LEFT)
        {
            SDL_Rect leftEdge = {coord.x, coord.y, cThickness, cTileSize};
            SDL_RenderFillRect(Window::instance()->getRenderer(), &leftEdge);
        }
        if (edgeFlags & Wall::UP_LEFT)
        {
            SDL_Rect ulCorner = {coord.x, coord.y, cThickness, cThickness};
            SDL_RenderFillRect(Window::instance()->getRenderer(), &ulCorner);
        }
        if (edgeFlags & Wall::UP_RIGHT)
        {
            SDL_Rect urCorner = {coord.x + cTileSize - cThickness, coord.y, cThickness, cThickness};
            SDL_RenderFillRect(Window::instance()->getRenderer(), &urCorner);
        }
        if (edgeFlags & Wall::DOWN_RIGHT)
        {
            SDL_Rect drCorner = {coord.x + cTileSize - cThickness, coord.y + cTileSize - cThickness,
                                 cThickness, cThickness};
            SDL_RenderFillRect(Window::instance()->getRenderer(), &drCorner);
        }
        if (edgeFlags & Wall::DOWN_LEFT)
        {
            SDL_Rect dlCorner = {coord.x, coord.y + cTileSize - cThickness, cThickness, cThickness};
            SDL_RenderFillRect(Window::instance()->getRenderer(), &dlCorner);
        }
    }
    else
    {
        const int halfTile = cTileSize / 2;
        CoordXY<double> delta = wall.getDistanceToShip();
        int thickScale = 40000;
        int diffx1 = static_cast<int>(delta.x);
        int diffy1 = static_cast<int>(delta.y);
        int diffx2 = diffx1 - halfTile;
        int diffy2 = diffy1 - halfTile;
        diffx1 *= diffx1;
        diffx2 *= diffx2;
        diffy1 *= diffy1;
        diffy2 *= diffy2;
        int ulThickness = 0;
        if ((diffx1 + diffy1 + 1) <= thickScale)
        {
            ulThickness = std::min(cTileSize, thickScale / (diffx1 + diffy1 + 1));
        }
        int urThickness = 0;
        if ((diffx2 + diffy1 + 1) <= thickScale)
        {
            urThickness = std::min(cTileSize, thickScale / (diffx2 + diffy1 + 1));
        }
        int dlThickness = 0;
        if ((diffx1 + diffy2 + 1) <= thickScale)
        {
            dlThickness = std::min(cTileSize, thickScale / (diffx1 + diffy2 + 1));
        }
        int drThickness = 0;
        if ((diffx2 + diffy2 + 1) <= thickScale)
        {
            drThickness = std::min(cTileSize, thickScale / (diffx2 + diffy2 + 1));
        }
        if (edgeFlags & Wall::UP)
        {
            SDL_Rect ulEdge = {coord.x, coord.y, halfTile, ulThickness};
            SDL_Rect urEdge = {coord.x + halfTile, coord.y, halfTile, urThickness};
            SDL_RenderFillRect(Window::instance()->getRenderer(), &ulEdge);
            SDL_RenderFillRect(Window::instance()->getRenderer(), &urEdge);
        }
        if (edgeFlags & Wall::RIGHT)
        {
            SDL_Rect ruEdge = {coord.x + cTileSize - urThickness, coord.y, urThickness, halfTile};
            SDL_Rect rdEdge = {coord.x + cTileSize - drThickness, coord.y + halfTile, drThickness, halfTile};
            SDL_RenderFillRect(Window::instance()->getRenderer(), &ruEdge);
            SDL_RenderFillRect(Window::instance()->getRenderer(), &rdEdge);
        }
        if (edgeFlags & Wall::DOWN)
        {
            SDL_Rect dlEdge = {coord.x, coord.y + cTileSize - dlThickness, halfTile, dlThickness};                                                 
            SDL_Rect drEdge = {coord.x + halfTile, coord.y + cTileSize - drThickness, halfTile, drThickness};                                                 
            SDL_RenderFillRect(Window::instance()->getRenderer(), &dlEdge);
            SDL_RenderFillRect(Window::instance()->getRenderer(), &drEdge);
        }
        if (edgeFlags & Wall::LEFT)
        {
            SDL_Rect luEdge = {coord.x, coord.y, ulThickness, halfTile};
            SDL_Rect ldEdge = {coord.x, coord.y+halfTile, dlThickness, halfTile};
            SDL_RenderFillRect(Window::instance()->getRenderer(), &luEdge);
            SDL_RenderFillRect(Window::instance()->getRenderer(), &ldEdge);
        }
        if (edgeFlags & Wall::UP_LEFT)
        {
            SDL_Rect ulCorner = {coord.x, coord.y, ulThickness, ulThickness};
            SDL_RenderFillRect(Window::instance()->getRenderer(), &ulCorner);
        }
        if (edgeFlags & Wall::UP_RIGHT)
        {
            SDL_Rect urCorner = {coord.x + cTileSize - urThickness, coord.y, urThickness,
                                 urThickness};
            SDL_RenderFillRect(Window::instance()->getRenderer(), &urCorner);
        }
        if (edgeFlags & Wall::DOWN_RIGHT)
        {
            SDL_Rect drCorner = {coord.x + cTileSize - drThickness,
                                 coord.y + cTileSize - drThickness, drThickness, drThickness};
            SDL_RenderFillRect(Window::instance()->getRenderer(), &drCorner);
        }
        if (edgeFlags & Wall::DOWN_LEFT)
        {
            SDL_Rect dlCorner = {coord.x, coord.y + cTileSize - dlThickness, dlThickness,
                                 dlThickness};
            SDL_RenderFillRect(Window::instance()->getRenderer(), &dlCorner);
        }
    }
    
}

void DrawVisitor::drawDoor(const Door &door) const
{
    auto coord = door.getPosition();
    uint8_t red = 0, green = 0, blue = 0;
    switch (door.getColor())
    {
    case Color::MAGENTA:
        blue = 0xff;
    case Color::RED:
        red = 0xff;
        break;
    case Color::GREEN:
        green = 0xff;
        break;
    case Color::BLUE:
        blue = 0xff;
        break;
    }
    SDL_SetRenderDrawColor(Window::instance()->getRenderer(), red, green, blue, 0xff);
    SDL_Rect doorRect = {coord.x, coord.y, cTileSize, cTileSize};
    if (door.isOpen())
    {
        SDL_RenderDrawRect(Window::instance()->getRenderer(), &doorRect);
    }
    else
    {
        SDL_RenderFillRect(Window::instance()->getRenderer(), &doorRect);
    }
}

void DrawVisitor::drawSwitch(const Switch &swtch) const
{
    auto coord = swtch.getPosition();
    uint8_t red = 0, green = 0, blue = 0;
    switch (swtch.getColor())
    {
    case Color::MAGENTA:
        blue = 0xff;
    case Color::RED:
        red = 0xff;
        break;
    case Color::GREEN:
        green = 0xff;
        break;
    case Color::BLUE:
        blue = 0xff;
        break;
    }
    SDL_SetRenderDrawColor(Window::instance()->getRenderer(), red, green, blue, 0xff);
    SDL_Rect switchRect = {coord.x, coord.y, cTileSize * 2, cTileSize * 2};
    if (swtch.isPressed())
    {
        SDL_RenderDrawRect(Window::instance()->getRenderer(), &switchRect);
    }
    else
    {
        SDL_RenderFillRect(Window::instance()->getRenderer(), &switchRect);
    }
    Text::instance()->setColor(Color::WHITE);
    Text::instance()->placeChar('!', coord.x + 7, 2 + coord.y);
    Text::instance()->placeChar('!', coord.x + 8, 2 + coord.y);
}

void DrawVisitor::drawMover(const Mover &mover) const
{
    auto coord = mover.getPosition();
    SDL_Rect moverOutline = {coord.x, coord.y, mover.getWidth(), mover.getHeight()};
    SDL_Rect moverInterior = {coord.x + cThickness, coord.y + cThickness,
                              mover.getWidth() - cThickness * 2,
                              mover.getHeight() - cThickness * 2};
    if (mover.getColor() == Color::CYAN)
    {
        SDL_SetRenderDrawColor(Window::instance()->getRenderer(), 0x00, 0xff, 0xff, 0xff);
    }
    else
    {
        SDL_SetRenderDrawColor(Window::instance()->getRenderer(), 0xff, 0xff, 0x00, 0xff); 
    }
    SDL_RenderFillRect(Window::instance()->getRenderer(), &moverOutline);
    SDL_SetRenderDrawColor(Window::instance()->getRenderer(), 0x00, 0x00, 0x00, 0x00);
    SDL_RenderFillRect(Window::instance()->getRenderer(), &moverInterior);
}
