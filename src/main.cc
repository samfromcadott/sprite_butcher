#include <iostream>
#include "finders_interface.h"

import packing;

using namespace rectpack2D;

int main() {
	// Create an array of rectangles
	rect_array rectangles;

	rectangles.emplace_back(rect_xywh(0, 0, 20, 40));
	rectangles.emplace_back(rect_xywh(0, 0, 120, 40));
	rectangles.emplace_back(rect_xywh(0, 0, 85, 59));
	rectangles.emplace_back(rect_xywh(0, 0, 199, 380));
	rectangles.emplace_back(rect_xywh(0, 0, 85, 875));

	// Find their packed size
	auto [size, output] = pack_frames(rectangles);

	// Display results
	std::cout << "Resultant bin: " << size.w << " " << size.h << std::endl;

	for (const auto& r : output) {
		std::cout << r.x << " " << r.y << " " << r.w << " " << r.h << std::endl;
	}

	return 0;
}
