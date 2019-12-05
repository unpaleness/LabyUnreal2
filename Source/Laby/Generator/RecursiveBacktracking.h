#pragma once

#include "GeneratorBase.h"

class RecursiveBacktracking : public GeneratorBase {
public:
	RecursiveBacktracking();

	/** Generates maze using Recursive Backtracking algorithm */
	virtual LabyContainerPtr GenerateMaze(int32 NewHSize, int32 NewVSize) override;
};
