#include "connectfour.hpp"
#include <cmath>
#include <stdexcept>

ConnectFour::ConnectFour() {
    player = 1;
    gameComplete = false;
    draw = false;
}

bool ConnectFour::makeMove(short c) {
    // Returns whether or not the move was successfully made
    if (gameComplete)
        return false;

    if (!validColumn(c)) {
        return false;
    }

    short r = addChip(c);

    if (!checkWin(c, r)) {
        if (!checkDraw()) {
            player = (player == 1 ? 2 : 1);
        }
    }

    return true;
}

short ConnectFour::getPlayer() { return player; }

bool ConnectFour::getGameComplete() { return gameComplete; }

bool ConnectFour::getDraw() { return draw; }

short ConnectFour::getChip(short c, short r) { return board[c][r]; }

short ConnectFour::addChip(short c) {
    // Returns the filled row.
    for (short r = 5; r >= 0; r--) {
        if (board[c][r] == 0) {
            board[c][r] = player;
            return r;
        }
    }
    // The method shouldn't ever not return by the end of the for loop
    throw std::invalid_argument("Chip added to full column");
}

bool ConnectFour::columnFull(short c) { return board[c][0] != 0; }

bool ConnectFour::validColumn(short c) { return (0 <= c && c <= 6) && !columnFull(c); }

bool ConnectFour::checkDraw() {
    for (short r = 0; r < 6; r++) {
        for (short c = 0; c < 7; c++) {
            if (board[c][r] == 0) {
                return false;
            }
        }
    }

    gameComplete = true;
    draw = true;
    return true;
}

bool ConnectFour::checkWin(short c, short r) {
    if (checkWinVertical(c) || checkWinHorizontal(r) || checkWinDiagonal(c, r)) {
        gameComplete = true;
        return true;
    } else
        return false;
}

bool ConnectFour::checkWinVertical(short c) {
    short consecutive = 0;
    for (int r = 0; r < 6; r++) {
        if (board[c][r] == player) {
            consecutive++;
            if (consecutive == 4) {
                return true;
            }
        } else {
            consecutive = 0;
        }
    }
    return false;
}

bool ConnectFour::checkWinHorizontal(short r) {
    short consecutive = 0;
    for (short c = 0; c < 7; c++) {
        if (board[c][r] == player) {
            consecutive++;
            if (consecutive == 4) {
                return true;
            }
        } else {
            consecutive = 0;
        }
    }
    return false;
}

bool ConnectFour::checkWinDiagonal(short c, short r) {
    // Top left to bottom right check
    short consecutive = 0;
    short delta = std::min(c, r);
    for (short x = c - delta, y = r - delta; x < 7 && y < 6; x++, y++) {
        if (board[x][y] == player) {
            consecutive++;
            if (consecutive == 4) {
                return true;
            }
        } else {
            consecutive = 0;
        }
    }

    // Top right to bottom left check
    consecutive = 0;
    delta = std::min((short)(6 - c), r);
    for (short x = c + delta, y = r - delta; 0 <= x && y < 6; x--, y++) {
        if (board[x][y] == player) {
            consecutive++;
            if (consecutive == 4) {
                return true;
            }
        } else {
            consecutive = 0;
        }
    }

    return false;
}