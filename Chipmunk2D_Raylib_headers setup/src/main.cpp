#include "raylib.h"
#include "munk.h"  // Includes physics functions

int main() {
    InitWindow(800, 600, "Chipmunk2D + Raylib");
    SetTargetFPS(60);

    InitPhysics();  // Initialize physics engine

    while (!WindowShouldClose()) {
        UpdatePhysics(1.0 / 60.0);  // Update physics

        // Get the position of the box from Chipmunk2D
        cpVect boxPos = GetBoxPosition();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // ✅ Draw the physics box using Raylib (Rendering in main.cpp)
        DrawRectangle(boxPos.x - 25, boxPos.y - 25, 50, 50, BLUE);

        EndDrawing();
    }

    CleanupPhysics();  // Cleanup Chipmunk2D
    CloseWindow();
    
    return 0;
}


