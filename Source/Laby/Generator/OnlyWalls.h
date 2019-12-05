#pragma once

#include "GeneratorBase.h"

class OnlyWalls : public GeneratorBase {
public:
	OnlyWalls();

	/** Only creates all possible walls */
	virtual LabyContainerPtr GenerateMaze(int32 HSize, int32 VSize) override;
};
