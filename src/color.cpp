#include "color.hpp"

namespace mfight {

Color::Color(): c{0, 0, 0, 255} {}
Color::Color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a): c{r, g, b, a} {}

std::uint8_t Color::operator[](int component) const noexcept {
	return c[component];
}

std::uint8_t &Color::operator[](int component) noexcept {
	return c[component];
}

std::uint8_t Color::r() const noexcept {
	return c[Component::RED];
}

std::uint8_t &Color::r() noexcept {
	return c[Component::RED];
}

std::uint8_t Color::g() const noexcept {
	return c[Component::GREEN];
}

std::uint8_t &Color::g() noexcept {
	return c[Component::GREEN];
}

std::uint8_t Color::b() const noexcept {
	return c[Component::BLUE];
}

std::uint8_t &Color::b() noexcept {
	return c[Component::BLUE];
}

std::uint8_t Color::a() const noexcept {
	return c[Component::ALPHA];
}

std::uint8_t &Color::a() noexcept {
	return c[Component::ALPHA];
}

Color Color::from_hsv(float h, float s, float v, float a) noexcept {
	// Contruct Color from Hue, Saturation and Value
	float r = 0, g = 0, b = 0;

	if (s == 0) {
		r = g = b = v;
	} else {
		h /= 60;
		int i = (int)h;
		auto f = h - i, p = v * (1 - s), q = v * (1 - s * f), t = v * (1 - s * (1 - f));
		switch (i) {
		case 0:
			r = v;
			g = t;
			b = p;
			break;
		case 1:
			r = q;
			g = v;
			b = p;
			break;
		case 2:
			r = p;
			g = v;
			b = t;
			break;
		case 3:
			r = p;
			g = q;
			b = v;
			break;
		case 4:
			r = t;
			g = p;
			b = v;
			break;
		default:
			r = v;
			g = p;
			b = q;
			break;
		}
	}
	return Color(r * 255, g * 255, b * 255, a * 255);
}

} // namespace mfight
