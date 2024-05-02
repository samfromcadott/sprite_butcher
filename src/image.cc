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

	// Setter
	Color& operator()(const size_t x, const size_t y) {
		return pixels[ width * y + x ];
	}
	// Getter
	Color operator()(const size_t x, const size_t y) const {
		return pixels[ width * y + x ];
	}

	void save(const string& filename) {
		std::vector<unsigned char> image;

		for (auto pixel : pixels) {
			image.push_back(pixel.r);
			image.push_back(pixel.g);
			image.push_back(pixel.b);
			image.push_back(pixel.a);
		}

		lodepng::encode(filename.c_str(), image, width, height);
	}
};

/// This function saves the sprite sheet with random colors filling the frames
export bool save_sheet_debug(const string& filename, const rect_wh& canvas_size, const std::vector<rect_type>& frames) {
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

	return true;
}
