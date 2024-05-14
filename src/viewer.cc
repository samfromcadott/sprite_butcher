#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <raylib.h>

using namespace std;

struct SpriteFrame {
	uint16_t x, y, w, h, crop_x, crop_y;
};

vector<SpriteFrame> load_sprite_data(const string& filename) {
	ifstream file;
	file.open(filename);

	vector<SpriteFrame> frames;

	// Iterate over each row
	while ( !file.eof() ) {
		// Read the current row
		string row;
		getline(file, row);

		stringstream buffer(row); // Buffer for this row
		string temp; // Used to hold the text value
		vector<uint16_t> values;

		// Iterate over each column in this row
		while( getline(buffer, temp, '\t') ) {
			values.push_back( strtod(temp.c_str(), 0) );
		}

		if (values.size() < 6) continue; // Skip if the row wasn't read properly

		// Add the frame to the frames vector
		SpriteFrame frame;
		frame.x = values[0];
		frame.y = values[1];
		frame.w = values[2];
		frame.h = values[3];
		frame.crop_x = values[4];
		frame.crop_y = values[5];
		frames.push_back(frame);
	}

	frames.erase( frames.begin() ); // Remove the header row

	return frames;
}

void render_frame(Texture2D& sprite, const Vector2 position, const Frame& frame) {

}

int main() {
	const int screen_width = 600;
	const int screen_height = 512;

	InitWindow(screen_width, screen_height, "Sprite Sheet Viewer");

	SetTargetFPS(60);

	Texture2D sprite_sheet = LoadTexture("sample.png");
	auto frame_data = load_sprite_data("sample.png.tsv");

	while ( !WindowShouldClose() ) {
		BeginDrawing();

		ClearBackground(RAYWHITE);

		DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

		EndDrawing();
	}

	UnloadTexture(sprite_sheet);

	CloseWindow();

	return 0;
}
