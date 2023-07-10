#pragma once
#include <stdexcept>

template<typename T>
struct CoordXY
{
    T x;
    T y;
};

enum class Color
{
    BLACK,
    RED,
    YELLOW,
    GREEN,
    CYAN,
    BLUE,
    MAGENTA,
    WHITE
};

struct ColorTriplet
{
    union
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t rgb[3];
    };
};