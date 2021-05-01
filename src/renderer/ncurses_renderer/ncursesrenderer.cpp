#include "ncursesrenderer.hpp"
#include "../../localgamehandler.hpp"
#include <unistd.h>

bool NCursesRenderer::init() {
    // Initialize nCurses
    WINDOW *w = initscr();
    cbreak();
    noecho();
    curs_set(FALSE);
    nodelay(w, true);

    // Initialize nCurses mouse support
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
    keypad(w, TRUE);

    // Initialize nCurses colors
    hasColor = has_colors();
    if (hasColor) {
        start_color();
        init_pair(TEXT_COLOR::DEFAULT_COLOR, COLOR_WHITE, COLOR_BLACK);
        init_pair(TEXT_COLOR::RED, COLOR_RED, COLOR_BLACK);
        init_pair(TEXT_COLOR::YELLOW, COLOR_YELLOW, COLOR_BLACK);
        init_pair(TEXT_COLOR::GREEN, COLOR_GREEN, COLOR_BLACK);
    }

    // Initialize game handler
    gameHandler = std::make_unique<LocalGameHandler>();
    gameHandler->startNewGame();
    return true;
}

void NCursesRenderer::render() {
    erase();

    renderBoard();

    refresh();
    usleep(1000000.0 / 60.0);
}

void NCursesRenderer::renderBoard() {
    if (canFitFancyBoard()) {
        renderFancyBoard();
    } else {
        renderCompactBoard();
    }
}

void NCursesRenderer::renderCompactBoard() {
    const int END_ROW = std::min(COMPACT_BOARD_HEIGHT, LINES - TEXT_HEIGHT);
    const int END_COL = std::max(COMPACT_BOARD_WIDTH, COMPACT_BOARD_WIDTH - COLS);
    const int V_BOARD_PADDING = std::max(LINES - COMPACT_BOARD_HEIGHT, TEXT_HEIGHT);

    renderInfoText(V_BOARD_PADDING - TEXT_HEIGHT);

    // Render held chip
    if (!gameHandler->getGameComplete()) {
        int hoverCol =
            gameHandler->getChipHoverPos() * (canFitPaddedBoard() ? PADDED_BOARD_WIDTH : COMPACT_BOARD_WIDTH);
        if (hoverCol < COLS && V_BOARD_PADDING - 1 > 0) {
            TEXT_COLOR heldChipColor = (gameHandler->getPlayer() == 1 ? TEXT_COLOR::RED : TEXT_COLOR::YELLOW);
            drawChar(hoverCol, V_BOARD_PADDING - 1, 'V', heldChipColor);
        }
    }

    // Render board
    for (int r = 0; r < END_ROW; r++) {
        for (int c = 0; c < END_COL; c++) {
            const int x = c * (1 + canFitPaddedBoard());
            switch (gameHandler->getChip(c, r)) {
            case 0:
                drawChar(x, V_BOARD_PADDING + r, '-');
                break;
            case 1:
                drawChar(x, V_BOARD_PADDING + r, 'X', TEXT_COLOR::RED);
                break;
            case 2:
                drawChar(x, V_BOARD_PADDING + r, 'O', TEXT_COLOR::YELLOW);
                break;
            }
        }
    }
}

void NCursesRenderer::renderFancyBoard() {
    const int V_BOARD_PADDING = LINES - FANCY_BOARD_HEIGHT;

    renderInfoText(V_BOARD_PADDING - TEXT_HEIGHT);

    // Render held chip
    if (!gameHandler->getGameComplete()) {
        int hoverCol = gameHandler->getChipHoverPos() * FANCY_BOARD_WIDTH;
        if (hoverCol < COLS && V_BOARD_PADDING - 1 > 0) {
            TEXT_COLOR heldChipColor = (gameHandler->getPlayer() == 1 ? TEXT_COLOR::RED : TEXT_COLOR::YELLOW);
            drawChar(hoverCol, V_BOARD_PADDING - 1, 'V', heldChipColor);
        }
    }

    // Render board
    for (int r = 0; r < 6; r++) {
        for (int ry = 0; ry < FANCY_BOARD_CELL_HEIGHT; ry++) {
            const int y = r * FANCY_BOARD_CELL_HEIGHT + ry + r + V_BOARD_PADDING;
            for (int c = 0; c < 7; c++) {
                for (int cx = 0; cx < FANCY_BOARD_CELL_WIDTH; cx++) {
                    const int x = c * FANCY_BOARD_CELL_WIDTH + c + cx;

                    switch (gameHandler->getChip(c, r)) {
                    case 0:
                        drawChar(x, y, '-');
                        break;
                    case 1:
                        drawChar(x, y, 'X', TEXT_COLOR::RED);
                        break;
                    case 2:
                        drawChar(x, y, 'O', TEXT_COLOR::YELLOW);
                        break;
                    }
                }
            }
        }
    }
}

void NCursesRenderer::renderInfoText(int y) {
    if (gameHandler->getGameComplete()) {
        if (gameHandler->getGameCancelled()) {
            drawString(0, y, "Game cancelled");
        } else if (gameHandler->getDraw()) {
            drawString(0, y, "Draw!");
        } else {
            drawString(0, y, gameHandler->getPlayerDisplayName(gameHandler->getPlayer()) + " Wins!",
                       (gameHandler->getPlayer() == 1 ? TEXT_COLOR::RED : TEXT_COLOR::YELLOW));
        }

        if (gameHandler->waitingForRematch()) {
            std::string rematchString = "Rematch";
            if (!gameHandler->hasVotedForRematch(true)) {
                rematchString += '?';
                drawString(YES_BUTTON_X, y + 1, "YES", TEXT_COLOR::GREEN);
                drawString(NO_BUTTON_X, y + 1, "NO", TEXT_COLOR::RED);
            }
            rematchString += " (";
            rematchString +=
                (gameHandler->hasVotedForRematch(true) || gameHandler->hasVotedForRematch(false) ? "1" : "0");
            rematchString += "/2)";
            drawString(0, y + 1, rematchString);
        } else {
            drawString(0, y + 1, "Press any key to quit");
        }
    } else {
        drawString(0, y, gameHandler->getPlayerDisplayName(gameHandler->getPlayer()) + "'s turn",
                   (gameHandler->getPlayer() == 1 ? TEXT_COLOR::RED : TEXT_COLOR::YELLOW));
    }
}

bool NCursesRenderer::canFitFancyBoard() {
    return LINES >= (TEXT_HEIGHT + FANCY_BOARD_HEIGHT) && COLS >= FANCY_BOARD_WIDTH;
}

bool NCursesRenderer::canFitPaddedBoard() { return COLS >= PADDED_BOARD_WIDTH; }

void NCursesRenderer::drawChar(int x, int y, char c) { drawChar(x, y, c, TEXT_COLOR::DEFAULT_COLOR); }

void NCursesRenderer::drawChar(int x, int y, char c, TEXT_COLOR color) {
    if (hasColor) {
        attron(COLOR_PAIR(color));
    }
    mvaddch(y, x, c);
}

void NCursesRenderer::drawString(int x, int y, std::string s) { drawString(x, y, s, TEXT_COLOR::DEFAULT_COLOR); }

void NCursesRenderer::drawString(int x, int y, std::string s, TEXT_COLOR color) {
    if (hasColor) {
        attron(COLOR_PAIR(color));
    }
    mvaddstr(y, x, s.c_str());
}

void NCursesRenderer::handleInputs() {
    handleMouse();

    int key = getch();
    if (key != ERR && key != KEY_MOUSE) {
        handleKeyPress(key);
    }
}

void NCursesRenderer::handleMouse() {
    MEVENT event;

    if (getmouse(&event) == OK) {
        bool clicked = (event.bstate & BUTTON1_CLICKED) || (event.bstate & BUTTON3_CLICKED);

        // If the user clicked and the game has finished, quit
        if (clicked && gameHandler->hasQuit()) {
            quit = true;
            return;
        }

        if (!gameHandler->getGameComplete()) {
            // Get board width
            int boardWidth;
            if (canFitFancyBoard()) {
                boardWidth = FANCY_BOARD_WIDTH;
            } else if (canFitPaddedBoard()) {
                boardWidth = PADDED_BOARD_WIDTH;
            } else {
                boardWidth = COMPACT_BOARD_WIDTH;
            }

            if (gameHandler->handlesCurrentPlayer()) {
                // Set what column is being hovered over
                gameHandler->setLocalHoverPos(static_cast<double>(event.x) / boardWidth);

                // If clicked (left or right mouse button) and in bounds, drop a
                // chip in the current column
                if (clicked) {
                    int hoverCol = (event.x * 7) / boardWidth;
                    if (0 <= hoverCol && hoverCol < 7) {
                        gameHandler->makeMove(hoverCol);
                    }
                }
            }
        } else if (gameHandler->waitingForRematch()) {
            if (!gameHandler->hasVotedForRematch(true)) {
                // Check if the user clicked "YES" or "NO" to vote for or
                // against a rematch.
                int voteButtonY;
                if (canFitFancyBoard()) {
                    const int V_BOARD_PADDING = LINES - FANCY_BOARD_HEIGHT;
                    voteButtonY = V_BOARD_PADDING - TEXT_HEIGHT + 1;
                } else {
                    const int V_BOARD_PADDING = std::max(LINES - COMPACT_BOARD_HEIGHT, TEXT_HEIGHT);
                    voteButtonY = V_BOARD_PADDING - TEXT_HEIGHT + 1;
                }

                if (event.y == voteButtonY) {
                    if (YES_BUTTON_X <= event.x && event.x < YES_BUTTON_X + 3) {
                        gameHandler->voteForRematch(true);
                    } else if(NO_BUTTON_X <= event.x && event.x < NO_BUTTON_X + 2) {
                        gameHandler->voteForRematch(false);
                    }
                }
            }
        }
    }
}

void NCursesRenderer::handleKeyPress(int key) {
    const int ESC_OR_ALT = 27;

    if (!gameHandler->hasQuit()) {
        switch (key) {
        case ESC_OR_ALT: {
            int secondKey = getch();
            // If secondKey is ERR, then the key pressed was ESCAPE
            // If secondKey is not ERR, then the key pressed was ALT
            if (secondKey == ERR) {
                quit = true;
                return;
            }
        }
        case 'q':
            quit = true;
            return;
        default:
            break;
        }
        if (!gameHandler->getGameComplete()) {
            switch (key) {
            case '1' ... '7':
                handleNumberKeyPress(key - '1');
                break;
            default:
                break;
            }
        } else if (gameHandler->waitingForRematch() && !gameHandler->hasVotedForRematch(true)) {
            switch (key) {
            case ' ':
            case KEY_ENTER:
            case 'y':
            case 'r':
                gameHandler->voteForRematch(true);
                break;
            case 'n':
                gameHandler->voteForRematch(false);
                break;
            default:
                break;
            }
        }
    } else {
        if (key != ERR) {
            quit = true;
        }
    }
}

void NCursesRenderer::handleNumberKeyPress(int k) { gameHandler->makeMove(k); }

void NCursesRenderer::close() {
    gameHandler->quit();
    endwin();
}