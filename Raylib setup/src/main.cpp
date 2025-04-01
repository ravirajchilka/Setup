#include "raylib.h"
#include <iostream>
#include <cstdint>

int main() {
	std::int16_t win_width = 800;
	std::int16_t win_height = 800;
	std::int16_t ball_x = 400;
	float ball_y = 400;
	Color mixed_green = {20,160,133,255};

	// Init game window
	InitWindow(win_width, win_height, "This is test window");
	SetTargetFPS(60);

	// Game loop
	while (!WindowShouldClose()) {

		// Event handling
		if(IsKeyDown(KEY_RIGHT) && IsKeyDown(KEY_UP)) {
			ball_x += 3;
			ball_y -= 3;
		} else if(IsKeyDown(KEY_LEFT)) {
			ball_x -= 3;
		} else if(IsKeyDown(KEY_DOWN)) {
			ball_y += 3;
		} else if(IsKeyDown(KEY_UP)){
			ball_y -= 3;
		}

		// Update
		
		// Redraw
		BeginDrawing();
		ClearBackground(mixed_green);
		//WaitTime(1000);
		DrawCircle(ball_x,ball_y,10,PINK);



		EndDrawing();
	}

	CloseWindow();

	return 0;
}
