#include <string>
#include <random>
#include <iostream>
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

	bool column_transparent(const size_t x) const {
		for (size_t y = 0; y < height-1; y++) {
			if ( (*this)(x, y).a > 0 ) return false;
		}

		return true;
	}

	bool row_transparent(const size_t y) const {
		for (size_t x = 0; x < width-1; x++) {
			if ( (*this)(x, y).a > 0 ) return false;
		}

		return true;
	}

public:
	rect_xywh crop_area;

	Image(){}
	Image(const size_t width, const size_t height) {
		pixels.resize(width * height);
		this->width = width;
		this->height = height;
	}
	Image(const string& filename) {
		load(filename);
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
		pixels.resize(width * height);

		for (size_t p = 0; p < width * height; p++) {
			size_t q = p * 4; // Each component is a separateitem in the `image` vector
			Color pixel = Color { image[q+0], image[q+1], image[q+2], image[q+3] };
			pixels[p] = pixel;
		}
	}

	/// Crops the excess transparency from the image. Returns the origin and size of the crop.
	rect_xywh crop() {
		int first_x = 0, first_y = 0;
		int last_x = width, last_y = height;

		// X-axis crop
		for (size_t x = 0; x < width-1; x++) {
			if ( column_transparent(x) ) continue;
			first_x = x;
			break;
		}

		for (size_t x = width-1; x > first_x; x--) {
			if ( column_transparent(x) ) continue;
			last_x = x;
			break;
		}

		// Y-axis crop
		for (size_t y = 0; y < height-1; y++) {
			if ( row_transparent(y) ) continue;
			first_y = y;
			break;
		}

		for (size_t y = height-1; y > first_y; y--) {
			if ( row_transparent(y) ) continue;
			last_y = y;
			break;
		}

		crop_area = rect_xywh(first_x, first_y, last_x-first_x, last_y-first_y);

		return crop_area;
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

/// Saves the sprite sheet
export void save_sheet(const string& filename, const rect_wh& canvas_size, const std::vector<Image>& frames, const std::vector<rect_type>& rectangles) {
	// Create an empty canvas
	Image canvas(canvas_size.w, canvas_size.h);

	// For each frame copy it onto the canvas
	for (size_t i = 0; i < frames.size(); i++) {
		rect_xywh crop_area = frames[i].crop_area;
		rect_xywh dest = rectangles[i]; // Destination of the frame

		// Copy pixels from each frame into its packed rectangle
		for (size_t x = crop_area.x; x < crop_area.x + crop_area.w; x++)
		for (size_t y = crop_area.y; y < crop_area.y + crop_area.h; y++) {
			auto dx = dest.x + x - crop_area.x;
			auto dy = dest.y + y - crop_area.y;

			canvas(dx, dy) = frames[i](x, y);
		}
	}

	// Save the canvas
	canvas.save(filename);
}
