#include "confirmbutton.hpp"

ConfirmButton::ConfirmButton() {}

bool ConfirmButton::init(SDL_Renderer *r, TTF_Font *font, int radius, bool confirm) {
    this->radius = radius;
    bool success = true;

    SDL_Color red = {0xFF, 0x00, 0x00, SDL_ALPHA_OPAQUE};
    texture.setRenderer(r);

    // https://wiki.libsdl.org/SDL_CreateRGBSurface
    static const int depth = 32;
    Uint32 rmask;
    Uint32 gmask;
    Uint32 bmask;
    Uint32 amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    SDL_Surface *buttonSurface = SDL_CreateRGBSurface(0, 64, 64, depth, rmask, gmask, bmask, amask);
    if (buttonSurface != NULL) {
        // Green for true, red for false
        Uint32 buttonColor = confirm ? SDL_MapRGBA(buttonSurface->format, 0x00, 0xFF, 0x00, SDL_ALPHA_OPAQUE)
                                     : SDL_MapRGBA(buttonSurface->format, 0xFF, 0x00, 0x00, SDL_ALPHA_OPAQUE);

        for (int x = 0; x < radius * 2; x++) {
            for (int y = 0; y < radius * 2; y++) {
                const int dx = radius - x;
                const int dy = radius - y;

                if ((dx * dx + dy * dy) <= (radius * radius)) {
                    // https://stackoverflow.com/questions/20070155/how-to-set-a-pixel-in-a-sdl-surface
                    Uint32 *const target_pixel = (Uint32 *)((Uint8 *)buttonSurface->pixels + y * buttonSurface->pitch +
                                                            x * buttonSurface->format->BytesPerPixel);
                    *target_pixel = buttonColor;
                }
            }
        }

        // Draw "Y" for 'yes' vote buttons, and "N" for 'no' vote buttons
        SDL_Color white = {0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE};
        SDL_Surface *textSurface = TTF_RenderText_Solid(font, (confirm ? "Y" : "N"), white);
        if (textSurface != NULL) {
            // Convert textSurface to a surface with a format that can be
            // blited on to buttonSurface
            SDL_Surface *convertedTextSurface = SDL_ConvertSurface(textSurface, buttonSurface->format, 0);
            if (convertedTextSurface != NULL) {
                SDL_Rect dstRect = {buttonSurface->w / 10, buttonSurface->h / 10, (8 * buttonSurface->w) / 10,
                                    (8 * buttonSurface->h) / 10};
                if (SDL_BlitScaled(convertedTextSurface, NULL, buttonSurface, &dstRect) == -1) {
                    printf("Unable to blit text surface onto button surface! SDL Error: %s\n", SDL_GetError());
                    success = false;
                }
                SDL_FreeSurface(convertedTextSurface);
            } else {
                printf("Unable to convert text surface for blitting onto button surface! SDL Error: %s\n",
                       SDL_GetError());
                success = false;
            }
            SDL_FreeSurface(textSurface);
        } else {
            printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
            success = false;
        }

        texture.getTextureFromSurface(buttonSurface);
        SDL_FreeSurface(buttonSurface);
    } else {
        printf("Unable to create surface! SDL Error: %s\n", SDL_GetError());
        success = false;
    }

    return success;
}

ConfirmButton::~ConfirmButton() { free(); }

int ConfirmButton::render() { return texture.render(x, y, 2 * radius, 2 * radius); }

void ConfirmButton::setPos(int x, int y) {
    this->x = x;
    this->y = y;
}

int ConfirmButton::getX() { return x; }

int ConfirmButton::getY() { return y; }

int ConfirmButton::getRadius() { return radius; }

bool ConfirmButton::withinButton(int x, int y) {
    return std::pow((this->x + radius) - x, 2) + std::pow((this->y + radius) - y, 2) <= radius * radius;
}

void ConfirmButton::free() { texture.free(); }