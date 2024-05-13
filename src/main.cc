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

	for ( auto& file : fs::directory_iterator(input_path) ) {
		auto filename = file.path().generic_string();
		images.push_back( Image(filename) );
	}

	// Crop transparent area from images
	std::cout << "Cropping..." << '\n';
	rect_array rectangles;

	for (auto& image : images) {
		rectangles.push_back( image.crop() );
	}

	// Find their packed size
	std::cout << "Packing..." << '\n';
	auto [size, output_rects] = pack_frames(rectangles);

	// Export the sprite sheet
	std::cout << "Exporting..." << '\n';

	// save_sheet_debug(output_path, size, output);
	save_sheet(output_path, size, images, output_rects);

	// Write a TSV file with the frame data
	std::cout << "Writing data sheet..." << '\n';

	ofstream data_sheet;
	data_sheet.open(output_path + ".tsv");
	data_sheet << "X\tY\tWIDTH\tHEIGHT\tCROP X\tCROP Y\n";

	for (size_t i = 0; i < images.size(); i++) {
		auto crop_area = images[i].crop_area;
		auto rect = output_rects[i];

		data_sheet <<
			rect.x << "\t" <<
			rect.y << "\t" <<
			rect.w << "\t" <<
			rect.h << "\t" <<
			crop_area.x << "\t" <<
			crop_area.y << "\n";
	}

	std::cout << "Done." << '\n';

	return 0;
}
