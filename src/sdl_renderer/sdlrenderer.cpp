#include "sdlrenderer.hpp"
#include "../localgamehandler.hpp"

bool SDLRenderer::init() {
    // Initialize SDL and SDL_ttf
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    if (TTF_Init() < 0) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }

    // Create window and get screen surface
    win = SDL_CreateWindow("Connect Four", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                           SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (win == NULL) {
        printf("SDL window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    } else {
        screenSurface = SDL_GetWindowSurface(win);
    }

    // Create renderer
    renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        printf("SDL renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Initialize game handler
    gameHandler = std::make_unique<LocalGameHandler>();
    gameHandler->startNewGame();

    // Initialize size constants
    boardCellSize = std::min((SCREEN_WIDTH * 0.8) / 7, (SCREEN_HEIGHT * 0.8) / 7.5);
    heldChipHeaderHeight = 1.5 * boardCellSize;
    hBoardPadding = (SCREEN_WIDTH - (7 * boardCellSize)) / 2;
    vBoardPadding = (SCREEN_HEIGHT - (7.5 * boardCellSize)) / 2;
    boardWidth = boardCellSize * 7;
    boardHeight = boardCellSize * 6;
    chipRadius = (boardCellSize * 0.75) / 2;

    // Initialize text display textures
    font = TTF_OpenFont("resources/font.ttf", vBoardPadding * 0.8);
    if (font == NULL) {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }

    player1TurnText.setRenderer(renderer);
    player2TurnText.setRenderer(renderer);
    player1WinText.setRenderer(renderer);
    player2WinText.setRenderer(renderer);
    gameCancelledText.setRenderer(renderer);
    drawText.setRenderer(renderer);

    rematchVoteNoVotesText.setRenderer(renderer);
    rematchVoteLocalVoteText.setRenderer(renderer);
    rematchVoteOpponentVoteText.setRenderer(renderer);
    waitingToQuitText.setRenderer(renderer);

    player1TurnText.getTextureFromText(font, (gameHandler->getPlayerDisplayName(1) + "'s turn").c_str(), red);
    player2TurnText.getTextureFromText(font, (gameHandler->getPlayerDisplayName(2) + "'s turn").c_str(), yellow);
    player1WinText.getTextureFromText(font, (gameHandler->getPlayerDisplayName(1) + " Wins!").c_str(), red);
    player2WinText.getTextureFromText(font, (gameHandler->getPlayerDisplayName(2) + " Wins!").c_str(), yellow);
    gameCancelledText.getTextureFromText(font, "Game cancelled", black);
    drawText.getTextureFromText(font, "Draw!", orange);

    rematchVoteNoVotesText.getTextureFromText(font, "Rematch? (0/2)", black);
    rematchVoteLocalVoteText.getTextureFromText(font, "Rematch (1/2)", black);
    rematchVoteOpponentVoteText.getTextureFromText(font, "Rematch? (1/2)", black);
    waitingToQuitText.getTextureFromText(font, "Press any key to quit", black);

    // Initialize rematch vote buttons
    voteForRematchButton.init(renderer, font, 32, true);
    voteAgainstRematchButton.init(renderer, font, 32, false);

    int voteForRematchButtonX = voteForRematchButton.getRadius() / 4,
        voteForRematchButtonY = (SCREEN_HEIGHT / 2) - voteForRematchButton.getRadius(),
        voteAgainstRematchButtonX = SCREEN_WIDTH - (2.25 * voteAgainstRematchButton.getRadius()),
        voteAgainstRematchButtonY = (SCREEN_HEIGHT / 2) - voteAgainstRematchButton.getRadius();

    voteForRematchButton.setPos(voteForRematchButtonX, voteForRematchButtonY);
    voteAgainstRematchButton.setPos(voteAgainstRematchButtonX, voteAgainstRematchButtonY);

    return true;
}

void SDLRenderer::mainloop() {
    while (!quit) {
        gameHandler->tick();
        handleInputs();
        render();
    }
}

void SDLRenderer::render() {
    clear(lightGray);

    int dropChipX = hBoardPadding + (gameHandler->getChipHoverPos() * boardWidth);

    // Render board
    drawRect(boardColor, {hBoardPadding, vBoardPadding + heldChipHeaderHeight, boardWidth, boardHeight});

    int hoverCol;
    if (gameHandler->handlesCurrentPlayer() && hBoardPadding <= mouseX && mouseX <= (boardWidth + hBoardPadding)) {
        hoverCol = (mouseX - hBoardPadding) / boardCellSize;
    } else {
        hoverCol = -1;
    }

    if (!gameHandler->getGameComplete()) {
        // Render held chip
        drawFilledCircle(gameHandler->getPlayer() == 1 ? red : yellow,
                         {dropChipX, vBoardPadding + (heldChipHeaderHeight / 2)}, chipRadius);

        if (gameHandler->handlesCurrentPlayer()) {
            // Render hovered colomn
            if (gameHandler->validColumn(hoverCol)) {
                drawRect(hoveredBoardColor, {hBoardPadding + (hoverCol * boardCellSize),
                                             vBoardPadding + heldChipHeaderHeight, boardCellSize, boardHeight});
            }
        }
    }

    // Render chips in board
    for (short col = 0; col < 7; col++) {
        for (short row = 0; row < 6; row++) {
            short chip = gameHandler->getChip(col, row);
            SDL_Color chipColor;
            if (chip == 0) {
                if (gameHandler->handlesCurrentPlayer() && !gameHandler->getGameComplete() && col == hoverCol &&
                    gameHandler->validColumn(col)) {
                    chipColor = white;
                } else {
                    chipColor = lightGray;
                }
            } else if (chip == 1) {
                chipColor = red;
            } else {
                chipColor = yellow;
            }
            drawFilledCircle(chipColor,
                             {hBoardPadding + ((col + 0.5) * boardCellSize),
                              vBoardPadding + (row + 0.5) * boardCellSize + heldChipHeaderHeight},
                             chipRadius);
        }
    }

    // Render text
    if (gameHandler->getGameComplete()) {
        if (gameHandler->getGameCancelled()) {
            gameCancelledText.render(hBoardPadding, 0, boardWidth, vBoardPadding);
        } else if (gameHandler->getDraw()) {
            drawText.render(hBoardPadding, 0, boardWidth, vBoardPadding);
        } else {
            if (gameHandler->getPlayer() == 1) {
                player1WinText.render(hBoardPadding, 0, boardWidth, vBoardPadding);
            } else {
                player2WinText.render(hBoardPadding, 0, boardWidth, vBoardPadding);
            }
        }

        if (gameHandler->waitingForRematch()) {
            if (gameHandler->hasVotedForRematch(true)) {
                rematchVoteLocalVoteText.render(hBoardPadding, SCREEN_HEIGHT - vBoardPadding, boardWidth,
                                                vBoardPadding);
            } else {
                voteForRematchButton.render();
                voteAgainstRematchButton.render();

                if (gameHandler->hasVotedForRematch(false)) {
                    rematchVoteOpponentVoteText.render(hBoardPadding, SCREEN_HEIGHT - vBoardPadding, boardWidth,
                                                       vBoardPadding);
                } else {
                    rematchVoteNoVotesText.render(hBoardPadding, SCREEN_HEIGHT - vBoardPadding, boardWidth,
                                                  vBoardPadding);
                }
            }
        } else {
            waitingToQuitText.render(hBoardPadding, SCREEN_HEIGHT - vBoardPadding, boardWidth, vBoardPadding);
        }
    } else {
        if (gameHandler->getPlayer() == 1) {
            player1TurnText.render(hBoardPadding, 0, boardWidth, vBoardPadding);
        } else {
            player2TurnText.render(hBoardPadding, 0, boardWidth, vBoardPadding);
        }
    }

    update();
}

void SDLRenderer::handleInputs() {
    SDL_GetMouseState(&mouseX, NULL);
    gameHandler->setLocalHoverPos((mouseX - hBoardPadding) / (double)boardWidth);
    while (SDL_PollEvent(&e) != 0) {
        switch (e.type) {
        case SDL_QUIT:
            quit = true;
            break;
        case SDL_MOUSEBUTTONDOWN:
            handleClick(e.button);
            break;
        case SDL_KEYDOWN:
            handleKeyPress(e.key.keysym.sym);
            break;
        default:
            break;
        }
    }
}

void SDLRenderer::handleClick(SDL_MouseButtonEvent e) {
    if (gameHandler->hasQuit()) {
        quit = true;
    } else if (!gameHandler->getGameComplete()) {
        dropHeldChip();
    } else if (gameHandler->waitingForRematch() && !gameHandler->hasVotedForRematch(true)) {
        if (voteAgainstRematchButton.withinButton(e.x, e.y)) {
            gameHandler->voteForRematch(false);
        } else if (voteForRematchButton.withinButton(e.x, e.y)) {
            gameHandler->voteForRematch(true);
        }
    }
}

void SDLRenderer::handleKeyPress(SDL_Keycode keycode) {
    if (!gameHandler->hasQuit()) {
        switch (keycode) {
        case SDLK_ESCAPE:
            quit = true;
            return;
        case SDLK_q:
            quit = true;
            return;
        default:
            break;
        }
        if (!gameHandler->getGameComplete()) {
            switch (keycode) {
            case SDLK_SPACE:
            case SDLK_RETURN:
                dropHeldChip();
                break;
            case SDLK_1 ... SDLK_7:
                handleNumberKeyPress(keycode - SDLK_1);
                break;
            case SDLK_KP_1 ... SDLK_KP_7:
                handleNumberKeyPress(keycode - SDLK_KP_1);
                break;
            default:
                break;
            }
        } else if (gameHandler->waitingForRematch() && !gameHandler->hasVotedForRematch(true)) {
            switch (keycode) {
            case SDLK_SPACE:
            case SDLK_RETURN:
            case SDLK_y:
            case SDLK_r:
                gameHandler->voteForRematch(true);
                break;
            case SDLK_n:
                gameHandler->voteForRematch(false);
                break;
            default:
                break;
            }
        }
    } else {
        quit = true;
    }
}

bool SDLRenderer::dropHeldChip() {
    if (hBoardPadding <= mouseX && mouseX <= (boardWidth + hBoardPadding)) {
        int dropCol = (mouseX - hBoardPadding) / ((double)boardWidth / 7);
        if (0 <= dropCol && dropCol <= 6) {
            return gameHandler->makeMove(dropCol);
        }
    }
    return false;
}

void SDLRenderer::handleNumberKeyPress(short k) { gameHandler->makeMove(k); }

void SDLRenderer::close() {
    gameHandler->quit();

    SDL_DestroyWindow(win);
    win = NULL;

    TTF_CloseFont(font);

    player1TurnText.free();
    player2TurnText.free();
    player1WinText.free();
    player2WinText.free();
    drawText.free();
    gameCancelledText.free();
    rematchVoteNoVotesText.free();
    rematchVoteLocalVoteText.free();
    rematchVoteOpponentVoteText.free();
    waitingToQuitText.free();

    voteForRematchButton.free();
    voteAgainstRematchButton.free();

    TTF_Quit();
    SDL_Quit();
}

void SDLRenderer::clear() {
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);
}

void SDLRenderer::clear(SDL_Color &color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);
}

void SDLRenderer::drawFilledCircle(SDL_Color &color, SDL_Point center, int radius) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w; // horizontal offset
            int dy = radius - h; // vertical offset
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer, center.x + dx, center.y + dy);
            }
        }
    }
}

void SDLRenderer::drawRect(SDL_Color &color, SDL_Rect rect) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
}

void SDLRenderer::update() { SDL_RenderPresent(renderer); }