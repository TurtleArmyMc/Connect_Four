#include "texturewrapper.hpp"
#include <algorithm>

TextureWrapper::TextureWrapper() {
    renderer = nullptr;
    texture = nullptr;
    height = 0;
    width = 0;
}

int TextureWrapper::getWidth() { return width; }

int TextureWrapper::getHeight() { return height; }

SDL_Rect TextureWrapper::getRect(int x, int y) { return {x, y, width, height}; }

void TextureWrapper::setRenderer(SDL_Renderer *r) { renderer = r; }

bool TextureWrapper::getTextureFromSurface(SDL_Surface *surface) {
    if (surface != NULL) {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (texture == NULL) {
            printf("Unable to create texture from rendered surface! SDL Error: %s\n", SDL_GetError());
            return false;
        }
        width = surface->w;
        height = surface->h;
        return true;
    } else {
        printf("Tried to create texture from NULL surface!");
        return false;
    }
}

bool TextureWrapper::getTextureFromText(TTF_Font *font, const char *text, SDL_Color color) {
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, color);
    if (textSurface != NULL) {
        bool ret = getTextureFromSurface(textSurface);
        SDL_FreeSurface(textSurface);
        return ret;
    } else {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }
}

int TextureWrapper::render(int x, int y, int maxWidth, int maxHeight) {
    if (renderer != nullptr) {
        double scale = std::min(std::min(((double)maxWidth) / width, ((double)maxHeight) / height), 1.0);
        int scaledWidth = width * scale;
        int scaledHeight = height * scale;
        SDL_Rect destRect = {x, y, scaledWidth, scaledHeight};

        return SDL_RenderCopy(renderer, texture, NULL, &destRect);
    } else {
        return -1;
    }
}

void TextureWrapper::free() {
    if (texture != NULL) {
        SDL_DestroyTexture(texture);
        texture = NULL;
        width = 0;
        height = 0;
    }
}

TextureWrapper::~TextureWrapper() { free(); }