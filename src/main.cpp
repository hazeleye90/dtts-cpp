#include "window.h"

#include "controller.h"
#include "drawing.h"
#include "text.h"
#include <iostream>
#include <bitset>
#include <format>
#include "timer.h"
#include "level.h"
#include "levelFactory.h"
#include "scene.h"
#include "levelScene.h"

namespace
{
static Timer t;

void handlekeypress(SDL_Event *e)
{
}

void mainloop()
{
    int frameCount = 0;
    t.start();
    bool quit = false;
    SDL_Event e;
    SceneManager m;
    int index = 0;

    LevelScene *s = new LevelScene(index);
    m.goTo(s);
    
    while (!quit)
    {
        bool nextLevel = false;
        bool prevLevel = false;
        while (SDL_PollEvent(&e) != 0)
        {
            switch (e.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                if (e.key.keysym.sym == SDLK_RIGHT)
                    nextLevel = true;
                else if (e.key.keysym.sym == SDLK_LEFT)
                    prevLevel = true;
                break;
            case SDL_JOYAXISMOTION:
                ControlState::instance()->updateAnalog(&e);
                break;
            case SDL_JOYBUTTONDOWN:
            case SDL_JOYBUTTONUP:
            case SDL_JOYHATMOTION:
                ControlState::instance()->updateDigital(&e);
                break;
            default:
                break;
            }
        }
        double fps = 0;
        if (t.getTicks() > 0)
        {
            fps = frameCount / (t.getTicks() / 1000.0);
        }
        Window::instance()->clearScreen();
        m.getScene()->update();
        m.getScene()->render();
        const auto lstick = ControlState::instance()->getLeftStick();/*
        Text::instance()->setColor(Color::GREEN);
        Text::instance()->placeString(std::format("{:2.5f} {:2.5f}", lstick.x, lstick.y), 30, 30,
                                      4);
        Text::instance()->setColor(Color::CYAN);
        Text::instance()->placeString(std::format("{:2.2f}", fps), 30, 80, 3);*/
        Window::instance()->update();
        frameCount += 1;
        if (m.getScene()->complete() || nextLevel)
        {
            LevelScene *s = new LevelScene((++index) % 39);
            m.goTo(s);
        }
        if (prevLevel)
        {
            LevelScene *s = new LevelScene((--index) % 39);
            m.goTo(s);

        }
    }
}
} // namespace

int main(int argc, char** argv)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0)
    {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }
    if (Window::instance()->init() && ControlState::instance()->init())
    {
        mainloop();
    }

    Window::instance()->close();
    return 0;
}