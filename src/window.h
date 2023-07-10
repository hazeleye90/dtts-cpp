#pragma once
#include <SDL.h>
#include <memory>

class Text;
class Window
{
public:
    static Window *instance();
    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;
    bool init();
    void close();
    void clearScreen();
    void update();
    SDL_Window *getWindow() const { return mWindow; }
    SDL_Renderer *getRenderer() const { return mRenderer; }

private:
    Window() = default;
    SDL_Window *mWindow = NULL;
    SDL_Renderer *mRenderer = NULL;
};