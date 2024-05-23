#ifndef MFIGHT_RENDER_HPP__
#define MFIGHT_RENDER_HPP__ 1

extern "C" {
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
}

#include "color.hpp"
#include <cstdint>
#include <string>
#include <string_view>

namespace mfight {

// A wrapper class for SDL_Surface
class Surface {
	SDL_Surface *_surface;

	void _check_surface() const;
	void _check_range(int x, int y) const;

public:
	Surface(SDL_Surface *surface);
	~Surface();

	// move constructor
	Surface(Surface &&other) noexcept;
	// move assignment operator
	Surface &operator=(Surface &&other) noexcept;

	// copy constructor is deleted
	// copy assignment operator is deleted

	SDL_Surface *native_handle() const noexcept;
	bool empty() const noexcept;
	int width() const;
	int height() const;
	Color pixel(int x, int y) const;
	void set_pixel(int x, int y, Color color) const;

	void fill(Color color) const;
	void blit(Surface &src, int src_x, int src_y, int dst_x, int dst_y, int width, int height) const;

	friend void swap(Surface &a, Surface &b) noexcept;
};

// A wrapper class for SDL_Texture
class Texture {
	SDL_Texture *_texture;

public:
	Texture(SDL_Texture *texture);
	~Texture();

	// move constructor
	Texture(Texture &&other) noexcept;
	// move assignment operator
	Texture &operator=(Texture &&other) noexcept;

	// copy constructor is deleted
	// copy assignment operator is deleted

	SDL_Texture *native_handle() const noexcept;
	bool empty() const noexcept;

	friend void swap(Texture &a, Texture &b) noexcept;
};

// A wrapper class for TTF_Font
class TTFFont {
	TTF_Font *_font;

public:
	TTFFont(std::string_view path, int size);
	TTFFont(TTF_Font *font);
	~TTFFont();

	// move constructor
	TTFFont(TTFFont &&other) noexcept;
	// move assignment operator
	TTFFont &operator=(TTFFont &&other) noexcept;

	// copy constructor is deleted
	// copy assignment operator is deleted

	TTF_Font *native_handle() const noexcept;
	bool empty() const noexcept;

	Surface render_text(std::string_view text, Color color) const;

	friend void swap(TTFFont &a, TTFFont &b) noexcept;
};

// A wrapper class for SDL_Window and SDL_Renderer
// It is responsible for creating the window and renderer,
// and also for handling the events.
class RenderWindow {
	SDL_Window *_window;
	SDL_Renderer *_renderer;
	int _width, _height;

	void _init_window(std::string_view title);
	void _init_renderer();

public:
	RenderWindow(std::string_view title, int width, int height);
	~RenderWindow();

	// move constructor
	RenderWindow(RenderWindow &&other) noexcept;
	// move assignment operator
	RenderWindow &operator=(RenderWindow &&other) noexcept;

	// copy constructor is deleted
	// copy assignment operator is deleted

	SDL_Window *native_window() const noexcept;
	SDL_Renderer *native_renderer() const noexcept;

	// getters
	int width() const noexcept;
	int height() const noexcept;

	// setters
	void set_title(std::string_view title) const;
	void set_size(int width, int height);

	// Clears the renderer with the given color
	// if no color is given, it clears with black
	void clear(Color color = {}) const;

	// present the renderer to the screen
	void present() const;

	// draws a filled rectangle
	// x, y, w, h are the position and size of the rectangle
	// color is the color of the rectangle
	void draw_rect(int x, int y, int w, int h, Color color) const;

	Texture create_texture(Surface &surface) const;

	// draws a texture
	// x, y, w, h are the position and size of the texture
	// texture is the texture to be drawn
	void draw_texture(Texture &texture, int x, int y, int w, int h) const;

	// draws a text
	// x, y, w, h are the position and size of the text
	// font is the font to be used
	// text is the text to be drawn
	// color is the color of the text
	void draw_text(std::string_view text, int x, int y, Color color, TTFFont &font) const;

	// Polls for SDL events
	bool poll_event(SDL_Event &event) const;
	bool have_event() const;

	friend void swap(RenderWindow &a, RenderWindow &b) noexcept;
};

} // namespace mfight

#endif // MFIGHT_RENDER_HPP__
