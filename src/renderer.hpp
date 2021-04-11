#pragma once

#include "gamehandler.hpp"
#include <memory>

class Renderer {
  protected:
    bool quit = false;
    std::unique_ptr<GameHandler> gameHandler;

  public:
    virtual bool init() = 0;
    virtual void mainloop() = 0;
    virtual void close() = 0;
};