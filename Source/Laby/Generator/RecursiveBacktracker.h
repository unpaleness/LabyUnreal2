#pragma once

#include "Base.h"

namespace Generator {

class RecursiveBacktracker : public Base {
public:
	RecursiveBacktracker();

	/** Generates maze using Recursive Backtracking algorithm */
	virtual LabyContainerPtr GenerateMaze(int32 NewHSize, int32 NewVSize) override;
};

}  // namespace Generator
