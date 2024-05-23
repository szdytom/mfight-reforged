#include "render.hpp"
#include <fmt/core.h>

int main() {
	mfight::TTFFont font("/usr/share/fonts/TTF/Hack-Regular.ttf", 14);
	mfight::RenderWindow win("MFight", 800, 600);

	bool should_close = false;
	int cur_x = 50, cur_y = 50;
	while (!should_close) {
		win.clear(mfight::Colors::WHITE);
		win.draw_rect(cur_x, cur_y, 20, 40, mfight::Colors::BLUE);
		win.draw_text(fmt::format("x: {} y: {}", cur_x, cur_y), 0, 0, mfight::Colors::BLACK, font);
		win.present();

		SDL_Event event;
		while (win.poll_event(event)) {
			if (event.type == SDL_QUIT) {
				should_close = true;
			} else if (event.type == SDL_KEYDOWN) {
                fmt::print("Key: {}\n", event.key.keysym.sym);
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					should_close = true;
                } else if (event.key.keysym.sym == SDLK_LEFT) {
                    cur_x -= 5;
                } else if (event.key.keysym.sym == SDLK_RIGHT) {
                    cur_x += 5;
                } else if (event.key.keysym.sym == SDLK_UP) {
                    cur_y -= 5;
                } else if (event.key.keysym.sym == SDLK_DOWN) {
                    cur_y += 5;
                }
            }
		}
	}
	return 0;
}
