#include <string>
#include <random>
#include <finders_interface.h>
#include <lodepng.h>

export module image;

import packing;

using namespace std;
using namespace rectpack2D;

// Used for generating random colors
static std::mt19937 gen;
static std::uniform_int_distribution<int> dist(0, 255);

export struct Color {
	unsigned char r = 0, g = 0, b = 0, a = 0;
};

export class Image {
private:
	vector<Color> pixels;
	size_t width = 0, height = 0;

public:
	Image(){}
	Image(const size_t width, const size_t height) {
		pixels.resize(width * height);
		this->width = width;
		this->height = height;
	}

	size_t get_width() const { return width; }
	size_t get_height() const { return height; }

	// Setter
	Color& operator()(const size_t x, const size_t y) {
		return pixels[ width * y + x ];
	}
	// Getter
	Color operator()(const size_t x, const size_t y) const {
		return pixels[ width * y + x ];
	}

	void save(const string& filename) const {
		vector<unsigned char> image;

		for (auto pixel : pixels) {
			image.push_back(pixel.r);
			image.push_back(pixel.g);
			image.push_back(pixel.b);
			image.push_back(pixel.a);
		}

		lodepng::encode(filename.c_str(), image, width, height);
	}

	void load(const string& filename) {
		pixels.clear(); // Remove any previously created image

		// Load an image file
		vector<unsigned char> image;
		unsigned int w, h;
		lodepng::decode(image, w, h, filename);

		width = w;
		height = h;

		for (size_t p = 0; p < width * height; p+=4) {
			Color pixel = Color { image[p+0], image[p+1], image[p+2], image[p+3] };
			pixels.push_back(pixel);
		}
	}

	/// Crops the excess transparency from the image. Returns the origin and size of the crop.
	rect_xywh crop() {
		return rect_xywh(0, 0, 0, 0);
	}
};

/// Saves the sprite sheet with random colors filling the frames
export void save_sheet_debug(const string& filename, const rect_wh& canvas_size, const std::vector<rect_type>& frames) {
	// Create an empty canvas
	Image canvas(canvas_size.w, canvas_size.h);

	// For each frame choose a random color and draw it on the canvas
	for (auto frame : frames) {
		unsigned char red = dist(gen);
		unsigned char green = dist(gen);
		unsigned char blue = dist(gen);
		Color color = Color {red, green, blue, 255};

		for (int x = frame.x; x < frame.x + frame.w; x++)
		for (int y = frame.y; y < frame.y + frame.h; y++) {
			canvas(x, y) = color;
		}
	}

	// Save the canvas
	canvas.save(filename);
}
