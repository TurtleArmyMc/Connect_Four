#ifdef SDL_RENDERER
#include "renderer/sdl_renderer/sdlrenderer.hpp"
#endif

#ifdef NCURSES_RENDERER
#include "renderer/ncurses_renderer/ncursesrenderer.hpp"
#endif

int main(int argc, char *args[]) {
#ifdef SDL_RENDERER
    std::unique_ptr<Renderer> renderer = std::make_unique<SDLRenderer>();
#endif

#ifdef NCURSES_RENDERER
    std::unique_ptr<Renderer> renderer = std::make_unique<NCursesRenderer>();
#endif

    if (!renderer->init()) {
        printf("Could not initialize renderer!\n");
        return -1;
    }

    renderer->mainloop();

    renderer->close();

    return 0;
}