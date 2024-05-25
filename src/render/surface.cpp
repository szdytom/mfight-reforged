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

void Surface::_check_surface() const MFIGHT_DEBUG_EXCEPT {
#ifdef DEBUG
	if (_surface == nullptr) {
		throw std::logic_error("Surface is null");
	}
#endif // DEBUG
}

void Surface::_check_range([[maybe_unused]] int x, [[maybe_unused]] int y) const MFIGHT_DEBUG_EXCEPT {
#ifdef DEBUG
	_check_surface();
	if (x < 0 || x >= _surface->w || y < 0 || y >= _surface->h) {
		throw std::out_of_range(fmt::format("Pixel coordinates out of range: ({}, {}) for surface of size ({}, {})", x,
											y, width(), height()));
	}
#endif // DEBUG
}

bool Surface::must_lock() const MFIGHT_DEBUG_EXCEPT {
	_check_surface();
	return SDL_MUSTLOCK(_surface);
}

void Surface::lock() const {
	_check_surface();
	if (!must_lock()) {
		return;
	}
	if (SDL_LockSurface(_surface) != 0) {
		throw std::runtime_error(fmt::format("Failed to lock surface: {}", SDL_GetError()));
	}
}

void Surface::unlock() const {
	_check_surface();
	SDL_UnlockSurface(_surface);
}

Color Surface::pixel(int x, int y) const {
	_check_range(x, y);

	lock();
	uint32_t pixel_value = static_cast<uint32_t *>(_surface->pixels)[y * _surface->pitch / 4 + x];
	unlock();

	uint8_t r, g, b, a;
	SDL_GetRGBA(pixel_value, _surface->format, &r, &g, &b, &a);
	return {r, g, b, a};
}

void Surface::set_pixel(int x, int y, Color color) const {
	_check_range(x, y);

	uint32_t pixel_value = SDL_MapRGBA(_surface->format, color.r(), color.g(), color.b(), color.a());

	lock();
	static_cast<uint32_t *>(_surface->pixels)[y * _surface->pitch / 4 + x] = pixel_value;
	unlock();
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