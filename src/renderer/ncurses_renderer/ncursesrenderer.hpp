#pragma once

#include "../renderer.hpp"
#include <ncurses.h>
#include <string>

class NCursesRenderer : public Renderer {
  private:
    enum TEXT_COLOR { DEFAULT_COLOR = 1, RED, YELLOW, GREEN };
    bool hasColor;

    const int TEXT_HEIGHT = 2; // Current player name & voting info
    const int FANCY_BOARD_CELL_HEIGHT = 5;
    const int FANCY_BOARD_CELL_WIDTH = 7;
    const int FANCY_BOARD_HEIGHT = FANCY_BOARD_CELL_HEIGHT * 6 + 5;
    const int FANCY_BOARD_WIDTH = FANCY_BOARD_CELL_WIDTH * 7 + 6;
    const int COMPACT_BOARD_HEIGHT = 6;
    const int COMPACT_BOARD_WIDTH = 7;
    const int PADDED_BOARD_WIDTH = (2 * COMPACT_BOARD_WIDTH) - 1;
    const int YES_BUTTON_X = 15;
    const int NO_BUTTON_X = 20;

    virtual void render() override;
    void renderBoard();
    void renderCompactBoard();
    void renderFancyBoard();
    void renderInfoText(int y);
    bool canFitFancyBoard();
    bool canFitPaddedBoard();
    void drawChar(int x, int y, char c, TEXT_COLOR color);
    void drawChar(int x, int y, char c);
    void drawString(int x, int y, std::string s, TEXT_COLOR color);
    void drawString(int x, int y, std::string s);

    virtual void handleInputs() override;
    void handleKeyPress(int key);
    void handleNumberKeyPress(int);
    void handleMouse();

  public:
    virtual bool init() override;
    virtual void close() override;
};