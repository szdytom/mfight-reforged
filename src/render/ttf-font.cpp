#include "render.hpp"
#include <exception>
#include <fmt/core.h>
#include <utility>

namespace mfight {

TTFFont::TTFFont(std::string_view path, int size): _font(TTF_OpenFont(path.data(), size)) {
	if (!_font) {
		throw std::runtime_error(fmt::format("Cannot load font from {}: {}", path, TTF_GetError()));
	}
}

TTFFont::TTFFont(TTF_Font *font): _font(font) {}

TTFFont::~TTFFont() {
	TTF_CloseFont(_font);
}

TTFFont::TTFFont(TTFFont &&other) noexcept: _font(std::exchange(other._font, nullptr)) {}

TTFFont &TTFFont::operator=(TTFFont &&other) noexcept {
	TTF_CloseFont(_font);
	_font = std::exchange(other._font, nullptr);
	return *this;
}

TTF_Font *TTFFont::native_handle() const noexcept {
	return _font;
}

bool TTFFont::empty() const noexcept {
	return _font == nullptr;
}

Surface TTFFont::render_text(std::string_view text, Color color) const {
	SDL_Surface *surf
		= TTF_RenderUTF8_Blended(_font, text.data(), SDL_Color{color.r(), color.g(), color.b(), color.a()});
	if (!surf) {
		throw std::runtime_error(fmt::format("Cannot render text(TTF_RenderUTF8_Blended): {}", TTF_GetError()));
	}
	return Surface(surf);
}

void swap(TTFFont &lhs, TTFFont &rhs) noexcept {
    using std::swap;
    swap(lhs._font, rhs._font);
}

} // namespace mfight
