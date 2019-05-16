#pragma once

#include "GeneratorBase.h"

class OnlyWalls : public GeneratorBase {
public:
	OnlyWalls();

	/** Fills all walls */
	virtual void GenerateMaze() override;
};
