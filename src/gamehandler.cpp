#include "gamehandler.hpp"

bool GameHandler::validColumn(short c) { return _game.validColumn(c); }

short GameHandler::getPlayer() { return _game.getPlayer(); }

bool GameHandler::getGameComplete() { return _gameCancelled || _game.getGameComplete(); }

bool GameHandler::getGameCancelled() { return _gameCancelled; }

bool GameHandler::getDraw() { return _game.getDraw(); }

short GameHandler::getChip(short c, short r) { return _game.getChip(c, r); }

bool GameHandler::hasQuit() { return _quit; }

bool GameHandler::handlesCurrentPlayer() { return handlesPlayer(_game.getPlayer()); }

void GameHandler::setLocalHoverPos(double pos) {
    if (pos < 0) {
        _localHoverPos = 0;
    } else if (1 < pos) {
        _localHoverPos = 1;
    } else {
        _localHoverPos = pos;
    }
}