#include "text.h"
#include "window.h"
#include <iostream>
#include <cstdint>

namespace
{
static const std::string fontPath = "../../data/font.bmp"; // TODO: Fix this, seriously
static const std::string fontMap = "0123456789abcdefghijklmnopqrstuvwxyz.,!?-+/'\"<>^_ ";
} // namespace

static Text *textInstance = nullptr;

Text* Text::instance()
{
    if (!textInstance)
    {
        textInstance = new Text();
        textInstance->init();
    }
    return textInstance;
}

bool Text::init()
{
    SDL_Surface *loadedSurface = SDL_LoadBMP(fontPath.c_str());
    if (!loadedSurface)
    {
        return false;
    }
    SDL_SetColorKey(loadedSurface, SDL_TRUE, 0);
    mTexture = SDL_CreateTextureFromSurface(Window::instance()->getRenderer(), loadedSurface);
    if (!mTexture)
    {
        return false;
    }
    // Create map of letter textures based on the input bmp
    // Each letter is in a row. Letters are black-on-white with a vertical gray line separating them.
    // This block reads the first row of pixels and adds an entry to mLetterInfo telling it the coordinates
    // and size of each respetive letter in the texture.
    const int wid = loadedSurface->w;
    const int hi = loadedSurface->h;
    const uint8_t delimiter = 164;
    int leftPix = 0;
    int index = 0;
    const int bpp = loadedSurface->format->BytesPerPixel;
    for (int i = 0; i < wid && index < fontMap.length(); i++)
    {
        uint8_t *p = (uint8_t *)loadedSurface->pixels + i * bpp;
        if (*p == delimiter)
        {
            mLetterClips[fontMap[index]] = SDL_Rect{leftPix, 0, i - leftPix, hi};
            leftPix = i+1;
            index++;   
        }
    }
    SDL_FreeSurface(loadedSurface);
    return true;
}

const SDL_Rect &Text::getLetterClip(const char c)
{
    if (c >= 'A' && c <= 'Z')
        return mLetterClips[c + 'a' - 'A']; // Make lowercase
    return mLetterClips[c];
}
// Places a character on the screen with x, y being the coordinates of the top-left corner.
// Returns the expected x coordinate of the next letter.
int Text::placeChar(char c, int x, int y, int scale) 
{ 
    auto clipRect = &getLetterClip(c);
    SDL_Rect renderRect{x, y, clipRect->w * scale, clipRect->h * scale};
    SDL_RenderCopy(Window::instance()->getRenderer(), mTexture, clipRect, &renderRect);
    return x + (clipRect->w + 1) * scale;
}

// Places a string on the screen with x, y being the coordinates of the top-left corner.
// Returns the expected x coordinate of the next letter.
int Text::placeString(const std::string &s, int x, int y, int scale)
{
    for (char c : s)
    {
        x = placeChar(c, x, y, scale);
    }
    return x;
}

// Return the length of the string in pixels
int Text::getStringWidth(const std::string &s, int scale) 
{ 
    int width = 0;
    for (char c : s)
    {
        auto clip = &getLetterClip(c);
        width += clip->w + 1;
    }
    return (width - 1) * scale;
}

void Text::setColor(Color c) 
{
    switch (c)
    {
    case Color::BLACK:
        SDL_SetTextureColorMod(mTexture, 0x00, 0x00, 0x00);
        break;
    case Color::RED:
        SDL_SetTextureColorMod(mTexture, 0xFF, 0x00, 0x00);
        break;
    case Color::GREEN:
        SDL_SetTextureColorMod(mTexture, 0x00, 0xFF, 0x00);
        break;
    case Color::BLUE:
        SDL_SetTextureColorMod(mTexture, 0x00, 0x00, 0xFF);
        break;
    case Color::CYAN:
        SDL_SetTextureColorMod(mTexture, 0x00, 0xFF, 0xFF);
        break;
    case Color::MAGENTA:
        SDL_SetTextureColorMod(mTexture, 0xFF, 0x00, 0xFF);
        break;
    case Color::YELLOW:
        SDL_SetTextureColorMod(mTexture, 0xFF, 0xFF, 0x00);
        break;
    case Color::WHITE:
        SDL_SetTextureColorMod(mTexture, 0xFF, 0xFF, 0xFF);
        break;
    }
}
