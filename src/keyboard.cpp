#include "keyboard.h"
#include <SDL.h>
#include <assert.h>

static Keyboard *keyboard = nullptr;

Keyboard *Keyboard::instance()
{
    if (!keyboard)
    {
        keyboard = new Keyboard();
    }
    return keyboard;
}

void Keyboard::updateKeyStates()
{
    const Uint8 *keyStates = SDL_GetKeyboardState(NULL);
    mKeyStates = (static_cast<uint16_t>(keyStates[SDL_SCANCODE_UP]) << K_UP) |
                 (static_cast<uint16_t>(keyStates[SDL_SCANCODE_DOWN]) << K_DOWN) |
                 (static_cast<uint16_t>(keyStates[SDL_SCANCODE_LEFT]) << K_LEFT) |
                 (static_cast<uint16_t>(keyStates[SDL_SCANCODE_RIGHT]) << K_RIGHT) |
                 (static_cast<uint16_t>(keyStates[SDL_SCANCODE_SPACE]) << K_SPACE) |
                 (static_cast<uint16_t>(keyStates[SDL_SCANCODE_ESCAPE]) << K_ESCAPE) |
                 (static_cast<uint16_t>(keyStates[SDL_SCANCODE_RETURN]) << K_RETURN) |
                 (static_cast<uint16_t>(keyStates[SDL_SCANCODE_W]) << K_W) |
                 (static_cast<uint16_t>(keyStates[SDL_SCANCODE_S]) << K_S) |
                 (static_cast<uint16_t>(keyStates[SDL_SCANCODE_A]) << K_A) |
                 (static_cast<uint16_t>(keyStates[SDL_SCANCODE_D]) << K_D);
}

bool Keyboard::getKeyState(KeyCode k) 
{
    assert(k < 16);
    uint16_t mask = 1U << k; 
    return ((mKeyStates & mask) > 0);
}
