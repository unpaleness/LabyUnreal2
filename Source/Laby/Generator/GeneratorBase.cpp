#include "GeneratorBase.h"

DEFINE_LOG_CATEGORY(LogLabyGenerator)

GeneratorBase::GeneratorBase() {}

void GeneratorBase::Init(int32 NewX, int32 NewY, Walls* NewHWalls, Walls* NewVWalls, int32 NewMaxIterations) {
	X = NewX;
	Y = NewY;
	HWalls = NewHWalls;
	VWalls = NewVWalls;
	MaxIterations = NewMaxIterations;
}

void GeneratorBase::GenerateMaze() {}
