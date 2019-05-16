#pragma once

#include "Logging/LogMacros.h"
#include "Typedefs.h"

DECLARE_LOG_CATEGORY_EXTERN(LogLabyGenerator, Log, All)

class GeneratorBase {
public:
	GeneratorBase();

	void Init(int32 NewX, int32 NewY, Walls* NewHWalls, Walls* NewVWalls, int32 NewMaxIterations);
	/** Generates maze */
	virtual void GenerateMaze();

protected:
	int32 X = 1;
	int32 Y = 1;
	Walls* HWalls;
	Walls* VWalls;
	int32 MaxIterations = 10000;
};
