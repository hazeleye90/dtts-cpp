#pragma once
#include <cstdint>
enum KeyCode
{
    K_UP = 0,
    K_DOWN,
    K_LEFT,
    K_RIGHT,
    K_SPACE,
    K_ESCAPE,
    K_RETURN,
    K_W,
    K_S,
    K_A,
    K_D,
};

class Keyboard
{
public:
    Keyboard *instance();
    void updateKeyStates();
    bool getKeyState(KeyCode k);

private:
    uint16_t mKeyStates{0};
};