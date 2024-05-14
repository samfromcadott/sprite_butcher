#include "raylib.h"

int main() {
	const int screen_width = 600;
	const int screen_height = 512;

	InitWindow(screen_width, screen_height, "Sprite Sheet Viewer");

	SetTargetFPS(60);

	while ( !WindowShouldClose() ) {
		BeginDrawing();

		ClearBackground(RAYWHITE);

		DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
