#pragma once
#include <SDL.h>
#include <utility>
#include "util.h"
#include <cstdint>

enum ControlButton
{
    J_A = 0,
    J_B,
    J_X,
    J_Y,
    J_LB,
    J_RB,
    J_BACK,
    J_START,
    J_L3,
    J_R3,
    J_UP,
    J_RIGHT,
    J_DOWN,
    J_LEFT
};

class ControlState
{
public:
    static ControlState *instance();
    ControlState(const ControlState&) = delete;
    ControlState &operator=(const ControlState &) = delete;
    bool init();
    void updateAnalog(const SDL_Event *e);
    void updateDigital(const SDL_Event *e);
    const uint16_t getButtonStates() const { return mButtonStates; }
    const bool getButtonState(ControlButton button) const { return ((1U << button) & mButtonStates) > 0; }
    const CoordXY<double> getLeftStick() const
    {
        return CoordXY<double>{(lx + 0.5) / 32767.5, (ly + 0.5) / 32767.5};
    }
    const CoordXY<double> getRightStick() const
    {
        return CoordXY<double>{rx / 32768.0, ry / 32768.0};
    }
    const int getLeftTrigger() const { return lt; }
    const int getRightTrigger() const { return rt; }

private:
    ControlState() = default;
    uint16_t mButtonStates{0};
    union
    {
        struct
        {
            int lx;
            int ly;
            int rx;
            int ry;
            int lt;
            int rt;
        };
        int mSticks[6]{0, 0, 0, 0, 0, 0};
    };
    SDL_Joystick *mGameController = NULL;
};