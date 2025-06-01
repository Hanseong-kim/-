#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>
#include "gui.h"

using namespace std;

int main(int argc, char** argv) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        return 1;
    }
	TTF_Init();

	SDL_Window* window = SDL_CreateWindow(argv[0], SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 400, 400, 0);
	if (!window) {
		cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
		SDL_Quit();
		return 1;
	}
	SDL_Renderer* sdl_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!sdl_renderer) {
		cerr << "Could not get renderer: " << SDL_GetError() << endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	renderer r = sdl_renderer;
	checkered root;

	bool running = true;
    SDL_Event event;
	bool left_down = false;

	root.render(r);
	SDL_RenderPresent(sdl_renderer);
    while (running) {
		SDL_WaitEvent(&event);
		switch (event.type) {
		case SDL_QUIT:
			running = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			left_down = true;
			root.on_click(r, event.button.x, event.button.y);
			break;
		case SDL_MOUSEBUTTONUP:
			left_down = false;
			break;
		case SDL_MOUSEMOTION:
			if (left_down)
				root.on_drag(r, event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel);
			break;
		}
		if (root.needs_update()) {
			SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 255);
			SDL_RenderClear(sdl_renderer);
			root.render(r);
			SDL_RenderPresent(sdl_renderer);
		}
    }

    SDL_DestroyRenderer(sdl_renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

