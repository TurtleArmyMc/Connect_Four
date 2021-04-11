#pragma once

#include "gamehandler.hpp"

class LocalGameHandler : public GameHandler {
  protected:
    virtual bool handlesPlayer(short player) override;

  public:
    LocalGameHandler();
    virtual bool init() override;
    virtual void quit() override;
    virtual void tick() override;
    virtual bool waitingForRematch() override;
    virtual bool hasVotedForRematch(bool localPlayer) override;
    virtual bool voteForRematch(bool vote) override;
    virtual bool getRematchVote(bool localPlayer) override;
    virtual void startNewGame() override;
    virtual bool makeMove(short c) override;
    virtual std::string getPlayerDisplayName(short player) override;
    virtual double getChipHoverPos() override;
};