#include <iostream>
#include <filesystem>
#include <finders_interface.h>
#include <CLI/CLI.hpp>

import packing;
import image;

using namespace rectpack2D;
using namespace std;
namespace fs = std::filesystem;

int main(int argc, char** argv) {
	// Command line options
	CLI::App cli;
	argv = cli.ensure_utf8(argv);

	string input_path, output_path;

	cli.add_option("-i,--input", input_path, "Input frame directory")->required();
	CLI11_PARSE(cli, argc, argv);

	// List files in the sample directory
	for ( auto file : fs::directory_iterator(input_path) )
		std::cout << file.path() << std::endl;

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

	save_sheet_debug("test.png", size, output);

	return 0;
}
