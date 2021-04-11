#pragma once

#include "connectfour.hpp"
#include <string>

class GameHandler {
  protected:
    ConnectFour _game = ConnectFour();
    bool _gameCancelled = false;
    bool _quit = false;
    double _localHoverPos = 0;
    virtual bool handlesPlayer(short player) = 0;

  public:
    bool validColumn(short c);
    short getPlayer();
    bool getGameComplete();
    bool getGameCancelled();
    bool getDraw();
    short getChip(short c, short r);

    bool hasQuit();
    bool handlesCurrentPlayer();
    virtual bool makeMove(short c) = 0;
    virtual void setLocalHoverPos(double pos);
    virtual void startNewGame() = 0;
    virtual void quit() = 0;
    virtual double getChipHoverPos() = 0;
    virtual bool init() = 0;
    virtual void tick() = 0;
    virtual bool waitingForRematch() = 0;
    virtual bool hasVotedForRematch(bool localPlayer) = 0;
    virtual bool voteForRematch(bool vote) = 0;
    virtual bool getRematchVote(bool localPlayer) = 0;
    virtual std::string getPlayerDisplayName(short player) = 0;
};