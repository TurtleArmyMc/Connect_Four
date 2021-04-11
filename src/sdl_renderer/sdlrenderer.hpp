#pragma once

#include "../renderer.hpp"
#include "confirmbutton.hpp"
#include "texturewrapper.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class SDLRenderer : public Renderer {
  private:
    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 480;

    SDL_Color black{0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE};
    SDL_Color white{0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE};
    SDL_Color lightGray{0xEE, 0xEE, 0xEE, SDL_ALPHA_OPAQUE};
    SDL_Color red{0xFF, 0x00, 0x00, SDL_ALPHA_OPAQUE};
    SDL_Color yellow{0xFF, 0xFF, 0x00, SDL_ALPHA_OPAQUE};
    SDL_Color orange{0xFF, 0xA5, 0x00, SDL_ALPHA_OPAQUE};
    SDL_Color boardColor{0x00, 0x64, 0xFF, SDL_ALPHA_OPAQUE};        // Light blue
    SDL_Color hoveredBoardColor{0x00, 0x73, 0xFF, SDL_ALPHA_OPAQUE}; // Lighter blue

    SDL_Window *win = NULL;
    SDL_Surface *screenSurface = NULL;
    SDL_Renderer *renderer = NULL;
    TTF_Font *font = NULL;
    SDL_Event e;

    TextureWrapper player1TurnText;
    TextureWrapper player2TurnText;
    TextureWrapper player1WinText;
    TextureWrapper player2WinText;
    TextureWrapper gameCancelledText;
    TextureWrapper drawText;
    TextureWrapper rematchVoteNoVotesText;
    TextureWrapper rematchVoteLocalVoteText;
    TextureWrapper rematchVoteOpponentVoteText;
    TextureWrapper waitingToQuitText;

    ConfirmButton voteForRematchButton;
    ConfirmButton voteAgainstRematchButton;

    int boardCellSize;
    int heldChipHeaderHeight;
    int hBoardPadding;
    int vBoardPadding;
    int boardWidth;
    int boardHeight;
    int chipRadius;

    int mouseX;

    void handleInputs();
    void handleClick(SDL_MouseButtonEvent e);
    void handleKeyPress(SDL_Keycode keycode);
    void handleNumberKeyPress(short k);
    bool dropHeldChip();
    void render();
    void clear();
    void clear(SDL_Color &color);
    void drawFilledCircle(SDL_Color &color, SDL_Point center, int radius);
    void drawRect(SDL_Color &color, SDL_Rect rect);
    void update();

  public:
    virtual bool init() override;
    virtual void mainloop() override;
    virtual void close() override;
};