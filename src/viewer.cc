#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <raylib.h>

using namespace std;

struct SpriteFrame {
	uint16_t x, y, w, h, crop_x, crop_y;
};

struct Action {
	uint16_t start, end;
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

void render_frame(Texture2D& sprite, const Vector2 position, const SpriteFrame& frame) {
	Rectangle rect = Rectangle {
		static_cast<float>(frame.x),
		static_cast<float>(frame.y),
		static_cast<float>(frame.w),
		static_cast<float>(frame.h)
	};
	Vector2 p = Vector2 {position.x + static_cast<float>(frame.crop_x), position.y + static_cast<float>(frame.crop_y)};

	DrawTextureRec(sprite, rect, p, WHITE);
}

int main() {
	const int screen_width = 600;
	const int screen_height = 512;
	const int frame_rate = 12; // Frame rate used for sprite animation

	InitWindow(screen_width, screen_height, "Sprite Sheet Viewer");

	SetTargetFPS(60);

	// Load the srite sheet and frame data
	Texture2D sprite_sheet = LoadTexture("sample.png");
	auto frame_data = load_sprite_data("sample.png.tsv");

	// Animation action data
	map<string, Action> action;
	string current_action = "idle";

	action["idle"] = Action {0, 20};
	action["walk"] = Action {21, 35};
	action["die"] = Action {36, 78};
	action["attack"] = Action {79, 88};
	action["fall"] = Action {89, 89};
	action["bitten"] = Action {90, 90};
	action["stun"] = Action {91, 91};

	while ( !WindowShouldClose() ) {
		// Find the current frame
		int frame = frame_rate * GetTime();
		frame = frame % (action[current_action].end - action[current_action].start + 1) + action[current_action].start;

		BeginDrawing();

			ClearBackground(RAYWHITE);
			render_frame(sprite_sheet, Vector2 {0, 0}, frame_data[frame]);

		EndDrawing();
	}

	UnloadTexture(sprite_sheet);

	CloseWindow();

	return 0;
}
