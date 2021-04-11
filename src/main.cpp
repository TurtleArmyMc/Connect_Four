#include "renderer.hpp"
#include "sdl_renderer/sdlrenderer.hpp"

int main(int argc, char *args[]) {
    std::unique_ptr<Renderer> renderer;
    renderer = std::make_unique<SDLRenderer>();

    if (!renderer->init()) {
        printf("Could not initialize renderer!\n");
        return -1;
    }

    renderer->mainloop();

    renderer->close();

    return 0;
}