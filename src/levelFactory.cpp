#include "levelFactory.h"
#include "constants.h"
#include <SDL.h>
#include <iostream>
#include <map>
#include <algorithm>

namespace
{
static std::vector<LevelInfo> sLevelInfo = {
    {"out you go", "out_you_go.bmp"},
    {"zig zag", "zig_zag.bmp"},
    {"spiral", "spiral.bmp"},
    {"roses are red", "roses_are_red.bmp"},
    {"violets are blue", "violets_are_blue.bmp"},
    {"it takes two", "it_takes_two.bmp"},
    {"crossover", "crossover.bmp"},
    {"three's company", "three's_company.bmp"},
    {"criss cross", "criss_cross.bmp"},
    {"they move", "they_move.bmp"},
    {"sliders", "sliders.bmp"},
    {"between the lines", "between_the_lines.bmp"},
    {"who turned out the lights", "who_turned_out_the_lights.bmp"},
    {"find your way", "find_your_way.bmp"},
    {"force your way", "force_your_way.bmp"},
    {"tricky spiral", "tricky_spiral.bmp"},
    {"closing time", "closing_time.bmp"},
    {"following directions", "following_directions.bmp"},
    {"one plus one", "one_plus_one.bmp"},
    {"alternator", "alternator.bmp"},
    {"a test", "a_test.bmp"},
    {"watch the door", "watch_the_door.bmp"},
    {"around the block", "around_the_block.bmp"},
    {"round and round", "round_and_round.bmp"},
    {"ninety degrees", "ninety_degrees.bmp"},
    {"buddy system", "buddy_system.bmp"},
    {"syncopation", "syncopation.bmp"},
    {"be careful", "be_careful.bmp"},
    {"be quick about it", "be_quick_about_it.bmp"},
    {"detour", "detour.bmp"},
    {"don't trust it", "don't_trust_it.bmp"},
    {"technicolor", "technicolor.bmp"},
    {"hard to reach", "hard_to_reach.bmp"},
    {"weave", "weave.bmp"},
    {"sidestep", "sidestep.bmp"},
    {"uh oh", "uh_oh.bmp"},
    {"cursed switch", "cursed_switch.bmp"},
    {"near miss", "near_miss.bmp"},
    {"that's too much man", "that's_too_much_man.bmp"},
};

static std::map<uint8_t, char> sTileMappings = {
    {0xff, 'S'}, {0x00, ' '}, {0xa4, 'W'}, {0xd2, 'B'}, {0xec, 'b'},
    {0x4f, 'R'}, {0xef, 'r'}, {0x71, 'G'}, {0x3e, 'g'}, {0xd5, 'M'},
    {0x07, 'm'}, {0x08, 'y'}, {0xfb, 'Y'}, {0x09, 'c'}, {0xe8, 'C'},
};

static char getLevelTile(int x, int y, SDL_Surface* s)
{ 
    uint8_t *pixels = (uint8_t *)s->pixels;
    return sTileMappings[pixels[x + y * s->pitch]]; 
}

static void setLevelTile(int x, int y, SDL_Surface *s, uint8_t pix)
{
    uint8_t *pixels = (uint8_t *)s->pixels;
    pixels[x + y * s->pitch] = pix;
}

static void clearLevelTile(int x, int y, SDL_Surface *s)
{
    uint8_t *pixels = (uint8_t *)s->pixels;
    pixels[x + y * s->pitch] = 0;
}
}

bool LevelFactory::loadLevel(int index, Level &level) 
{
    if (index >= sLevelInfo.size())
    {
        return false;
    }
    level.mTitle = sLevelInfo[index].levelName;
    std::string baseLevelPath = "C:\\Users\\aldra\\source\\repos\\dtts-cpp\\data\\levels\\";
    std::string filePath = baseLevelPath + sLevelInfo[index].levelPath;
    SDL_Surface *surface = SDL_LoadBMP(filePath.c_str());
    if (!surface)
    {
        std::cout << "Failed to open level with file path " << filePath << std::endl;
        return false;
    }
    if (getLevelTile(0, 0, surface) == 'S')
    {
        level.mDarkLevel = true;
        setLevelTile(0, 0, surface, 0xa4);
    }
    const int hi = surface->h, wid = surface->w;
    for (int y = 0; y < hi; y++)
    {
        for (int x = 0; x < wid; x++)
        {
            char tile = getLevelTile(x, y, surface);
            CoordXY position =
                CoordXY<int>({x * cTileSize + cLevelLeft, y * cTileSize + cLevelTop});
            if ((tile == 'S') && (getLevelTile(x + 1, y, surface) == 'S') &&
                (getLevelTile(x, y + 1, surface) == 'S'))
            {
                level.mShipStartPosition = {(x + 1) * cTileSize + cLevelLeft,
                                            (y + 1) * cTileSize + cLevelTop};
                clearLevelTile(x, y, surface);
                clearLevelTile(x + 1, y, surface);
                clearLevelTile(x, y + 1, surface);
                clearLevelTile(x + 1, y + 1, surface);
            }
            else if (tile == 'W')
            {
                std::shared_ptr<Wall> w = std::make_shared<Wall>(position, &level);

                uint8_t edgeFlags = 0;
                if ((x == 0) || (getLevelTile(x - 1, y, surface) != 'W'))
                {
                    edgeFlags |= Wall::LEFT;
                }
                if (((x == 0) && (y == 0)) || (getLevelTile(x - 1, y - 1, surface) != 'W'))
                {
                    edgeFlags |= Wall::UP_LEFT;
                }
                if ((y == 0) || (getLevelTile(x, y - 1, surface) != 'W'))
                {
                    edgeFlags |= Wall::UP;
                }
                if (((x == (wid - 1)) && (y == 0)) || (getLevelTile(x + 1, y - 1, surface) != 'W'))
                {
                    edgeFlags |= Wall::UP_RIGHT;
                }
                if ((x == (wid - 1)) || (getLevelTile(x + 1, y, surface) != 'W'))
                {
                    edgeFlags |= Wall::RIGHT;
                }
                if (((x == (wid - 1)) && (y == (hi - 1))) || (getLevelTile(x + 1, y + 1, surface) != 'W'))
                {
                    edgeFlags |= Wall::DOWN_RIGHT;
                }
                if ((y == (hi - 1)) || (getLevelTile(x, y + 1, surface) != 'W'))
                {
                    edgeFlags |= Wall::DOWN;
                }
                if (((x == 0) && (y == (hi - 1))) ||
                    (getLevelTile(x - 1, y + 1, surface) != 'W'))
                {
                    edgeFlags |= Wall::DOWN_LEFT;
                }
                w->setEdgeFlags(edgeFlags);
                level.mWalls.push_back(w);
                level.mEntities.push_back(w);
            }
            else if (tile == 'R' || tile == 'G' || tile == 'B' || tile == 'M')
            {
                Color c = Color::BLACK;
                switch (tile)
                {
                case 'R':
                    c = Color::RED;
                    break;
                case 'G':
                    c = Color::GREEN;
                    break;
                case 'B':
                    c = Color::BLUE;
                    break;
                case 'M':
                    c = Color::MAGENTA;
                    break;
                }
                std::shared_ptr<Door> d = std::make_shared<Door>(position, c);
                if (c == Color::MAGENTA)
                {
                    d->setIsOpen(true);
                }
                level.mDoors.push_back(d);
                level.mEntities.push_back(d);
            }
            else if (tile == 'r' || tile == 'g' || tile == 'b' || tile == 'm')
            {
                Color c = Color::BLACK;
                switch (tile)
                {
                case 'r':
                    c = Color::RED;
                    break;
                case 'g':
                    c = Color::GREEN;
                    break;
                case 'b':
                    c = Color::BLUE;
                    break;
                case 'm':
                    c = Color::MAGENTA;
                    break;
                }
                std::shared_ptr<Switch> s = std::make_shared<Switch>(position, c, &level);
                clearLevelTile(x, y, surface);
                clearLevelTile(x + 1, y, surface);
                clearLevelTile(x, y + 1, surface);
                clearLevelTile(x + 1, y + 1, surface);
                if (c == Color::MAGENTA)
                {
                    s->setIsPressed(true);
                }
                level.mSwitches.push_back(s);
                level.mEntities.push_back(s);
            }
            else if (tile == 'C' || tile == 'c' || tile == 'Y' || tile == 'y')
            {
                Color c = Color::BLACK;
                int direction = 1;
                switch (tile)
                {
                case 'c':
                    direction = -1;
                case 'C':
                    c = Color::CYAN;
                    break;
                case 'y':
                    direction = -1;
                case 'Y':
                    c = Color::YELLOW;
                    break;
                }
                int tx = x, ty = y;
                while ((tile == getLevelTile(tx, ty, surface)) && (++tx < wid));
                while ((tile == getLevelTile(tx-1, ty, surface)) && (++ty < hi));
                
                int a = 0;
                for (int qy = y; qy < ty; qy++)
                {
                    for (int qx = x; qx < tx; qx++)
                    {
                        clearLevelTile(qx, qy, surface);
                    }
                }
                int moverWidth = (tx - x) * cTileSize;
                int moverHeight = (ty - y) * cTileSize;
                std::shared_ptr<Mover> m =
                    std::make_shared<Mover>(position, moverWidth, moverHeight, c, direction, &level);
                level.mMovers.push_back(m);
                level.mEntities.push_back(m);
            }
        }
    }
    SDL_FreeSurface(surface);
    auto compareFunc = [](std::shared_ptr<Entity> e) {
        if (e->isWall())
            return 0;
        else if (e->isDoor())
            return 1;
        else if (e->isSwitch())
            return 2;
        else if (e->isMover())
            return 3;
        return -1;
    };
    auto comp = [compareFunc](std::shared_ptr<Entity> e1, std::shared_ptr<Entity> e2) {
        return compareFunc(e1) < compareFunc(e2);
    };
    std::stable_sort(level.mEntities.begin(), level.mEntities.end(), comp);
    return true;
}
