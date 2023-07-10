#include "controller.h"
#include <iostream>

static ControlState *controlState = nullptr;

ControlState* ControlState::instance()
{
    if (!controlState)
    {
        controlState = new ControlState();
    }
    return controlState;
}

bool ControlState::init() 
{ 
    bool success = true;
    if (SDL_NumJoysticks() < 1)
    {
        std::cout << "Warning: No joysticks detected" << std::endl;
        success = false;
    }
    else
    {
        mGameController = SDL_JoystickOpen(0);
        if (!mGameController)
        {
            std::cout << "Unable to open game controller. SDL_Error: " << SDL_GetError()
                      << std::endl;
        }
    }
    return success;
}

void ControlState::updateAnalog(const SDL_Event *e)
{
    if ((e->jaxis.which == 0) && (e->jaxis.axis < 6))
    {
        mSticks[static_cast<int>(e->jaxis.axis)] = e->jaxis.value;
    }
}

void ControlState::updateDigital(const SDL_Event* e)
{
    if (e->type == SDL_JOYHATMOTION)
    {
        if (e->jhat.which == 0)
        {
            mButtonStates &= ~(0xFFFF << J_UP);
            mButtonStates |= (static_cast<uint16_t>(e->jhat.value) << J_UP);
        }
    }
    else
    {
        if (e->jbutton.which == 0)
        {
            uint16_t button = 1U << e->jbutton.button;
            if (e->jbutton.state)
            {
                mButtonStates |= button;
            }
            else
            {
                mButtonStates &= ~button;
            }
        }
    }
}