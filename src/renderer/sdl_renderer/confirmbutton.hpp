#pragma once

#include "texturewrapper.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class ConfirmButton {
  private:
    int x, y, radius;
    TextureWrapper texture;

  public:
    ConfirmButton();
    ~ConfirmButton();
    bool init(SDL_Renderer *r, TTF_Font *font, int radius, bool confirm);
    int render();
    void free();
    void setPos(int x, int y);
    int getX();
    int getY();
    int getRadius();
    bool withinButton(int x, int y);
};