#include "munk.h"
#include "chipmunk.h"

// Chipmunk2D physics space and objects
cpSpace *space;
cpBody *boxBody;
cpShape *boxShape;

// Initialize Chipmunk2D physics
void InitPhysics() {
    space = cpSpaceNew();
    cpSpaceSetGravity(space, cpv(0, 500));  // Gravity

    // Box properties
    cpFloat mass = 1.0;
    cpFloat width = 50.0, height = 50.0;
    cpFloat moment = cpMomentForBox(mass, width, height);

    // Create box body
    boxBody = cpSpaceAddBody(space, cpBodyNew(mass, moment));
    cpBodySetPosition(boxBody, cpv(400, 100));

    // Create box shape
    boxShape = cpSpaceAddShape(space, cpBoxShapeNew(boxBody, width, height, 0.0));
    cpShapeSetFriction(boxShape, 0.7);
}

// Update Chipmunk2D physics
void UpdatePhysics(float dt) {
    cpSpaceStep(space, dt);
}

// ✅ New function: Get position (used in main.cpp for drawing)
cpVect GetBoxPosition() {
    return cpBodyGetPosition(boxBody);
}

// Cleanup Chipmunk2D
void CleanupPhysics() {
    cpShapeFree(boxShape);
    cpBodyFree(boxBody);
    cpSpaceFree(space);
}
