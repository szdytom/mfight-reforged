#include "render.hpp"
#include <exception>
#include <fmt/core.h>
#include <utility>

namespace mfight {

Surface::Surface(SDL_Surface *surface): _surface(surface) {}

Surface::~Surface() {
	SDL_FreeSurface(_surface);
}

Surface::Surface(Surface &&other) noexcept: _surface(std::exchange(other._surface, nullptr)) {}

Surface &Surface::operator=(Surface &&other) noexcept {
	if (this != &other) {
		SDL_FreeSurface(_surface);
		_surface = std::exchange(other._surface, nullptr);
	}
	return *this;
}

SDL_Surface *Surface::native_handle() const noexcept {
	return _surface;
}

bool Surface::empty() const noexcept {
	return _surface == nullptr;
}

int Surface::width() const {
	_check_surface();
	return _surface->w;
}

int Surface::height() const {
	_check_surface();
	return _surface->h;
}

void Surface::_check_surface() const {
#ifdef DEBUG
	if (_surface == nullptr) {
		throw std::runtime_error("Surface is null");
	}
#endif // DEBUG
}

void Surface::_check_range(int x, int y) const {
#ifdef DEBUG
	_check_surface();
	if (x < 0 || x >= _surface->w || y < 0 || y >= _surface->h) {
		throw std::out_of_range(fmt::format("Pixel coordinates out of range: ({}, {}) for surface of size ({}, {})", x,
											y, width(), height()));
	}
#endif // DEBUG
}

Color Surface::pixel(int x, int y) const {
	_check_range(x, y);

	bool need_lock = SDL_MUSTLOCK(_surface);
	if (need_lock) {
		SDL_LockSurface(_surface);
	}

	uint32_t pixel_value = static_cast<uint32_t *>(_surface->pixels)[y * _surface->pitch / 4 + x];

	if (need_lock) {
		SDL_UnlockSurface(_surface);
	}

	uint8_t r, g, b, a;
	SDL_GetRGBA(pixel_value, _surface->format, &r, &g, &b, &a);
	return {r, g, b, a};
}

void Surface::set_pixel(int x, int y, Color color) const {
	_check_range(x, y);

	uint32_t pixel_value = SDL_MapRGBA(_surface->format, color.r(), color.g(), color.b(), color.a());

	bool need_lock = SDL_MUSTLOCK(_surface);
	if (need_lock) {
		SDL_LockSurface(_surface);
	}
	static_cast<uint32_t *>(_surface->pixels)[y * _surface->pitch / 4 + x] = pixel_value;

	if (need_lock) {
		SDL_UnlockSurface(_surface);
	}
}

void Surface::fill(Color color) const {
	_check_surface();
	if (SDL_FillRect(_surface, nullptr, SDL_MapRGBA(_surface->format, color.r(), color.g(), color.b(), color.a()))
		!= 0) {
		throw std::runtime_error(fmt::format("SDL_FillRect: {}", SDL_GetError()));
	}
}

void Surface::blit(Surface &src, int src_x, int src_y, int dst_x, int dst_y, int width, int height) const {
	_check_surface();
	SDL_Rect src_rect = {src_x, src_y, width, height};
	SDL_Rect dst_rect = {dst_x, dst_y, width, height};
	if (SDL_BlitSurface(src.native_handle(), &src_rect, _surface, &dst_rect) != 0) {
		throw std::runtime_error(fmt::format("SDL_BlitSurface: {}", SDL_GetError()));
	}
}

void swap(Surface &lhs, Surface &rhs) noexcept {
	using std::swap;
	swap(lhs._surface, rhs._surface);
}

} // namespace mfight