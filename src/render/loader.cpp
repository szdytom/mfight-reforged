#include "render.hpp"
#include <exception>
#include <fmt/core.h>

namespace {

class SDLLoader {
public:
	SDLLoader(const SDLLoader &) = delete;
	SDLLoader &operator=(const SDLLoader &) = delete;

	SDLLoader() {
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
			throw std::runtime_error(fmt::format("SDL_Init: {}", SDL_GetError()));
		}
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");
		if (TTF_Init() != 0) {
			throw std::runtime_error(fmt::format("TTF_Init: {}", TTF_GetError()));
		}
	}

	~SDLLoader() {
		SDL_Quit();
		TTF_Quit();
	}
};

SDLLoader _sdl_loader;

} // namespace
