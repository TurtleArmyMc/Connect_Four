#pragma once

#include "../gamehandler.hpp"
#include <memory>

class Renderer {
  protected:
    bool quit = false;
    std::unique_ptr<GameHandler> gameHandler;

    virtual void render() = 0;
    virtual void handleInputs() = 0;

  public:
    virtual bool init() = 0;
    virtual void close() = 0;
    virtual void mainloop() {
        while (!quit) {
            gameHandler->tick();
            handleInputs();
            render();
        }
    }
};