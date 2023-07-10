#include "window.h"
#include "text.h"
#include <iostream>
#include "constants.h"

static Window *window = nullptr;

Window *Window::instance()
{
    if (!window)
    {
        window = new Window();
    }
    return window;
}
bool Window::init()
{
    bool success = true;
    mWindow =
        SDL_CreateWindow("don't touch the sides", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                         cScreenWidth, cScreenHeight, SDL_WINDOW_SHOWN);
    if (mWindow == NULL)
    {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        success = false;
    }
    else
    {
        //mScreenSurface = SDL_GetWindowSurface(mWindow);
        mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (!mRenderer) 
        {
            std::cout << "Failed to create renderer! SDL_Error: " << SDL_GetError() << std::endl;
            success = false;
        }
        else
        {
            SDL_SetRenderDrawColor(mRenderer, 0x00, 0x00, 0x00, 0xff);
        }
    }
    
    return success;
}

void Window::clearScreen()
{
    SDL_SetRenderDrawColor(mRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(mRenderer);
}

void Window::update()
{
    //SDL_UpdateWindowSurface(mWindow);
    SDL_RenderPresent(mRenderer);
}

void Window::close()
{
    SDL_DestroyWindow(mWindow);
    mWindow = NULL;
    SDL_Quit();
    delete this;
}
