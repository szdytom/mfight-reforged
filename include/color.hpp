#ifndef MFIGHT_COLOR_HPP__
#define MFIGHT_COLOR_HPP__ 1

#include <cstdint>

namespace mfight {

class Color {
public:
	struct Component {
		static constexpr int RED = 0;
		static constexpr int GREEN = 1;
		static constexpr int BLUE = 2;
		static constexpr int ALPHA = 3;
	};

	Color() noexcept;
	Color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a) noexcept;

	std::uint8_t operator[](int component) const noexcept;
	std::uint8_t& operator[](int component) noexcept;

	std::uint8_t r() const noexcept;
	std::uint8_t& r() noexcept;

	std::uint8_t g() const noexcept;
	std::uint8_t& g() noexcept;

	std::uint8_t b() const noexcept;
	std::uint8_t& b() noexcept;

	std::uint8_t a() const noexcept;
	std::uint8_t& a() noexcept;

	static Color from_hsv(float h, float s, float v, float a) noexcept;

private:
	std::uint8_t c[4];
};

namespace Colors {

const Color BLACK(0, 0, 0, 255);
const Color WHITE(255, 255, 255, 255);
const Color RED(255, 0, 0, 255);
const Color GREEN(0, 255, 0, 255);
const Color BLUE(0, 0, 255, 255);
const Color YELLOW(255, 255, 0, 255);
const Color MAGENTA(255, 0, 255, 255);
const Color CYAN(0, 255, 255, 255);
const Color TRANSPARENT(0, 0, 0, 0);

} // namespace Colors

} // namespace mfight

#endif
