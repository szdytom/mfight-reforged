#include "render.hpp"
#include <algorithm>
#include <exception>
#include <fmt/core.h>
#include <utility>

namespace mfight {

RenderWindow::RenderWindow(std::string_view title, int width, int height): _width(width), _height(height) {
	_init_window(title);
	_init_renderer();
}

void RenderWindow::_init_window(std::string_view title) {
	_window = SDL_CreateWindow(title.data(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _width, _height,
							   SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL);
	if (_window == nullptr) {
		throw std::runtime_error(fmt::format("SDL_CreateWindow Error: {}", SDL_GetError()));
	}
}

void RenderWindow::_init_renderer() {
	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
	if (_renderer == nullptr) {
		throw std::runtime_error(fmt::format("SDL_CreateRenderer Error: {}", SDL_GetError()));
	}
}

RenderWindow::~RenderWindow() {
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
}

RenderWindow::RenderWindow(RenderWindow &&other) noexcept
	: _window(std::exchange(other._window, nullptr))
	, _renderer(std::exchange(other._renderer, nullptr))
	, _width(other._width)
	, _height(other._height) {}

RenderWindow &RenderWindow::operator=(RenderWindow &&other) noexcept {
	_window = std::exchange(other._window, nullptr);
	_renderer = std::exchange(other._renderer, nullptr);
	_width = other._width;
	_height = other._height;
	return *this;
}

int RenderWindow::width() const noexcept {
	return _width;
}
int RenderWindow::height() const noexcept {
	return _height;
}

SDL_Window *RenderWindow::native_window() const noexcept {
	return _window;
}
SDL_Renderer *RenderWindow::native_renderer() const noexcept {
	return _renderer;
}

void RenderWindow::set_title(std::string_view title) const {
	SDL_SetWindowTitle(_window, title.data());
}

void RenderWindow::set_size(int width, int height) {
	SDL_SetWindowSize(_window, width, height);
	_width = width;
	_height = height;
}

void RenderWindow::clear(Color color) const {
	SDL_SetRenderDrawColor(_renderer, color.r(), color.g(), color.b(), color.a());
	SDL_RenderClear(_renderer);
}

void RenderWindow::present() const {
	SDL_RenderPresent(_renderer);
}

void RenderWindow::draw_rect(int x, int y, int w, int h, Color color) const {
	SDL_Rect rect{x, y, w, h};
	SDL_SetRenderDrawColor(_renderer, color.r(), color.g(), color.b(), color.a());
	SDL_RenderFillRect(_renderer, &rect);
}

Texture RenderWindow::create_texture(Surface &surface) const {
	SDL_Texture *texture = SDL_CreateTextureFromSurface(_renderer, surface.native_handle());
	if (texture == nullptr) {
		throw std::runtime_error(fmt::format("SDL_CreateTextureFromSurface Error: {}", SDL_GetError()));
	}
	return Texture{texture};
}

void RenderWindow::draw_texture(Texture &texture, int x, int y, int w, int h) const {
	SDL_Rect rect{x, y, w, h};
	if (SDL_RenderCopy(_renderer, texture.native_handle(), nullptr, &rect) != 0) {
		throw std::runtime_error(fmt::format("SDL_RenderCopy Error: {}", SDL_GetError()));
	}
}

void RenderWindow::draw_text(std::string_view text, int x, int y, Color color, TTFFont &font) const {
	Surface surface = font.render_text(text, color);
	Texture texture = create_texture(surface);
	draw_texture(texture, x, y, surface.width(), surface.height());
}

bool RenderWindow::poll_event(SDL_Event &event) const {
	return SDL_PollEvent(&event);
}

bool RenderWindow::have_event() const {
	return SDL_PollEvent(nullptr);
}

void swap(RenderWindow &lhs, RenderWindow &rhs) noexcept {
	using std::swap;
	swap(lhs._window, rhs._window);
	swap(lhs._renderer, rhs._renderer);
	swap(lhs._width, rhs._width);
	swap(lhs._height, rhs._height);
}

} // namespace mfight
