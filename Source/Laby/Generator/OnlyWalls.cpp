#include "OnlyWalls.h"

OnlyWalls::OnlyWalls() {}

void OnlyWalls::GenerateMaze() {
	HWalls->Empty();
	VWalls->Empty();

	// Fill all walls
	HWalls->Init(true, (Y + 1) * X);
	VWalls->Init(true, Y * (X + 1));
}
