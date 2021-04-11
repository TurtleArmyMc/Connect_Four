#include "localgamehandler.hpp"
#include <stdexcept>

LocalGameHandler::LocalGameHandler() { _game = ConnectFour(); }

bool LocalGameHandler::handlesPlayer(short player) { return true; }

void LocalGameHandler::startNewGame() { _game = ConnectFour(); }

bool LocalGameHandler::makeMove(short c) {
    if (!handlesPlayer(_game.getPlayer())) {
        return false;
    }
    return _game.makeMove(c);
}

bool LocalGameHandler::waitingForRematch() { return _game.getGameComplete() && !_gameCancelled && !_quit; }

bool LocalGameHandler::hasVotedForRematch(bool localPlayer) { return false; }

bool LocalGameHandler::getRematchVote(bool localPlayer) { return false; }

bool LocalGameHandler::voteForRematch(bool vote) {
    if (vote) {
        _game = ConnectFour();
    } else {
        quit();
    }
    return true;
}

bool LocalGameHandler::init() { return true; }

void LocalGameHandler::quit() { _quit = true; }

std::string LocalGameHandler::getPlayerDisplayName(short player) {
    if (player == 1) {
        return "Player 1";
    } else if (player == 2) {
        return "Player 2";
    }
    throw std::invalid_argument("Invalid player '" + std::to_string(player) + "'.");
}

double LocalGameHandler::getChipHoverPos() { return _localHoverPos; }

void LocalGameHandler::tick() {}