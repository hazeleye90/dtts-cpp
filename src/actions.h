#pragma once
#include "controller.h"
#include "keyboard.h"

class Actions
{
    static CoordXY<double> MoveAnalog();
    static float BrakeAnalog();
    static bool MoveUp();
    static bool MoveRight();
    static bool MoveDown();
    static bool MoveLeft();
    static bool BrakeDigital();
    static bool Pause();
    static bool Unpause();
    static bool StartGame();
};