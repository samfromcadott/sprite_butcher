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
	cli.add_option("-o,--output", output_path, "Output sprite sheet path")->required();

	CLI11_PARSE(cli, argc, argv);

	// Load images from input_path
	std::cout << "Loading images..." << '\n';
	vector<Image> images;

	for ( auto file : fs::directory_iterator(input_path) ) {
		auto filename = file.path().generic_string();
		images.push_back( Image(filename) );
	}

	// Crop transparent area from images
	std::cout << "Cropping..." << '\n';
	rect_array rectangles;

	for (auto image : images) {
		rectangles.push_back( image.crop() );
	}


	// Find their packed size
	std::cout << "Packing..." << '\n';
	auto [size, output] = pack_frames(rectangles);

	// Display results
	// std::cout << "Resultant bin: " << size.w << " " << size.h << std::endl;
	//
	// for (const auto& r : output) {
	// 	std::cout << r.x << " " << r.y << " " << r.w << " " << r.h << std::endl;
	// }

	std::cout << "Exporting..." << '\n';
	save_sheet_debug(output_path, size, output);
	std::cout << "Done." << '\n';

	return 0;
}
