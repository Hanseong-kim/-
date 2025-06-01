#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>
#include <cstdio>
#include "gui.h"

using namespace std;

class left_side : public vertical {
	text *t;
	int num_excls = 0;
public:
	left_side(text *t) : vertical(new checkered(), t), t(t) {}
	void do_something() {
		this->num_excls++;
		char msg[1000];
		for (int i = 0; i < this->num_excls; i++)
			msg[i] = '!';
		msg[this->num_excls] = 0;
		this->t->set_text(msg);
	}
};

class right_side : public button {
	left_side *left;
public:
	right_side(SDL_Renderer* r, left_side *l) : button(new vertical(new text(r, "You can click up here!", 16, 1, 1, 1, 0.4, 0, 0), new text(r, "Or down here!", 16, 1, 1, 1, 0, 0.4, 0))), left(l) {}
	void on_click(renderer const &r_dont_care, unsigned int x_dont_care, unsigned int y_dont_care) override {
		left->do_something();
	}
};

int main(int argc, char** argv) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        return 1;
    }
	TTF_Init();

	SDL_Window* win = SDL_CreateWindow(argv[0], SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);
	if (!win) {
		cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
		SDL_Quit();
		return 1;
	}
	SDL_Renderer* sdl_renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	if (!sdl_renderer) {
		cerr << "Could not get renderer: " << SDL_GetError() << endl;
		SDL_DestroyWindow(win);
		SDL_Quit();
		return 1;
	}

	renderer r = sdl_renderer;
	left_side *l = new left_side(new text(sdl_renderer, " ", 30, 0, 0, 0, 0.5, 0.5, 0.5));
	two_windows root(
		new window(sdl_renderer, "Checkerboards WOW", new checkered()),
		new window(sdl_renderer, "Busy window", new horizontal(l, new right_side(sdl_renderer, l)))
	);
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
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}

