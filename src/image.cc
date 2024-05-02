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

/// This function saves the sprite sheet with random colors filling the frames
export bool save_sheet_debug(const string& filename, const rect_wh& canvas_size, const std::vector<rect_type>& frames) {
	// Create an empty canvas
	std::vector<unsigned char> image;
	image.resize(canvas_size.w * canvas_size.h * 4);

	// For each frame choose a random color and draw it on the canvas
	for (auto frame : frames) {
		auto red = dist(gen);
		auto green = dist(gen);
		auto blue = dist(gen);
		unsigned char alpha = 255;

		for (int x = frame.x; x < frame.x + frame.w; x++)
		for (int y = frame.y; y < frame.y + frame.h; y++) {
			image[ 4 * canvas_size.w * y + (4*x+0) ] = red;
			image[ 4 * canvas_size.w * y + (4*x+1) ] = green;
			image[ 4 * canvas_size.w * y + (4*x+2) ] = blue;
			image[ 4 * canvas_size.w * y + (4*x+3) ] = alpha;
		}
	}

	// Save the canvas
	lodepng::encode(filename.c_str(), image, canvas_size.w, canvas_size.h);

	return true;
}
