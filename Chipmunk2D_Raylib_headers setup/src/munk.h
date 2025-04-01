#ifndef MUNK_H
#define MUNK_H

#include "chipmunk.h"  // ✅ Required for cpVect

void InitPhysics();
void UpdatePhysics(float dt);
cpVect GetBoxPosition();  // ✅ Declare GetBoxPosition()
void CleanupPhysics();

#endif
