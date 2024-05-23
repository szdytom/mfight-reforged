#include "render.hpp"
#include <exception>
#include <fmt/core.h>
#include <utility>

namespace mfight {

Texture::Texture(SDL_Texture *texture): _texture(texture) {}

Texture::~Texture() {
	if (_texture != nullptr) {
		SDL_DestroyTexture(_texture);
	}
}

Texture::Texture(Texture &&other) noexcept: _texture(std::exchange(other._texture, nullptr)) {}

Texture &Texture::operator=(Texture &&other) noexcept {
	if (this != &other) {
		SDL_DestroyTexture(_texture);
		_texture = std::exchange(other._texture, nullptr);
	}
	return *this;
}

SDL_Texture *Texture::native_handle() const noexcept {
	return _texture;
}

bool Texture::empty() const noexcept {
	return _texture == nullptr;
}

void swap(Texture &lhs, Texture &rhs) noexcept {
	using std::swap;
	swap(lhs._texture, rhs._texture);
}

} // namespace mfight
