#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class TextureWrapper {
  private:
    SDL_Texture *texture = nullptr;
    SDL_Renderer *renderer = nullptr;
    int height = 0, width = 0;

  public:
    TextureWrapper();
    ~TextureWrapper();
    int getWidth();
    int getHeight();
    SDL_Rect getRect(int x, int y);
    void setRenderer(SDL_Renderer *r);
    bool getTextureFromSurface(SDL_Surface *surface);
    bool getTextureFromText(TTF_Font *font, const char *text, SDL_Color color);
    int render(int x, int y, int maxWidth, int maxHeight);
    void free();
};