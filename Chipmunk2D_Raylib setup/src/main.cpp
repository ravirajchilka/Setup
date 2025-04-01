#include "raylib.h"
#include <chipmunk.h> // Include Chipmunk2D
#include <cstdint>
#include <iostream>

class Box {
public:
    std::int16_t height, width;
    cpFloat mass = 1.0f;
    cpFloat moment;
    cpBody *body;
    cpShape *shape;

    Box(cpSpace *space, cpFloat x, cpFloat y, std::int16_t w, std::int16_t h) {
        width = w;
        height = h;

        // Calculate moment of inertia for the box
        moment = cpMomentForBox(mass, width, height);

        // Create dynamic body (must be added to the space before the shape)
        body = cpBodyNew(mass, moment);
        cpBodySetPosition(body, cpv(x, y));
        cpSpaceAddBody(space, body); // First, add the body to the space

        // Now, create the box shape
        shape = cpBoxShapeNew(body, (cpFloat)width, (cpFloat)height, 0.0f);
        cpShapeSetFriction(shape, 0.7f);

        // Finally, add the shape to the space
        cpSpaceAddShape(space, shape);
    }

    void draw_box() {
        cpVect pos = cpBodyGetPosition(body);
        DrawRectangle((int)pos.x - width / 2, (int)pos.y - height / 2, width, height, BLUE);
    }
};

int main() {
    InitWindow(800, 800, "Chipmunk2D Gravity Example");
    SetTargetFPS(60);

    // Initialize Chipmunk2D physics space
    cpSpace *space = cpSpaceNew();
    cpVect gravity = cpv(0.0f, 500.0f); // Gravity pulling down
    cpSpaceSetGravity(space, gravity);

    // Create a static ground body
    cpBody *groundBody = cpBodyNewStatic();
    cpShape *groundShape = cpSegmentShapeNew(groundBody, cpv(0, 750), cpv(800, 750), 0);
    cpShapeSetFriction(groundShape, 1.0f);

    cpSpaceAddBody(space, groundBody);  // Add ground body
    cpSpaceAddShape(space, groundShape); // Add ground shape

    // Create dynamic falling box
    Box box(space, 400.0f, 100.0f, 80, 60);

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime(); // Time step for simulation
        cpSpaceStep(space, deltaTime); // Update physics

        // Draw everything
        BeginDrawing();
        ClearBackground(LIGHTGRAY);

        // Draw ground
        DrawLine(0, 750, 800, 750, DARKGRAY);

        // Draw falling box
        box.draw_box();

        EndDrawing();
    }

    // Cleanup
    cpShapeFree(box.shape);
    cpBodyFree(box.body);
    cpShapeFree(groundShape);
    cpBodyFree(groundBody);
    cpSpaceFree(space);
    
    CloseWindow();
    return 0;
}
