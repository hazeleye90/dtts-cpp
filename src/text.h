#pragma once
#include <SDL.h>
#include <string>
#include <map>
#include "util.h"

class Text
{
public:
    bool init();
    static Text *instance();
    Text(const Text &) = delete;
    Text &operator=(const Text &) = delete;
    const SDL_Rect &getLetterClip(const char c);
    int placeChar(char c, int x, int y, int scale = 1);
    int placeString(const std::string &s, int x, int y, int scale = 1);
    int getStringWidth(const std::string &s, int scale = 1);
    void setColor(Color c);

private:
    Text() = default;
    SDL_Texture *mTexture = NULL;
    std::map<char, SDL_Rect> mLetterClips;
};