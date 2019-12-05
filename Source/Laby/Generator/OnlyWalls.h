#pragma once

#include "Base.h"

namespace Generator {

class OnlyWalls : public Base {
public:
	OnlyWalls();

	/** Only creates all possible walls */
	virtual LabyContainerPtr GenerateMaze(int32 HSize, int32 VSize) override;
};

}  // namespace Generator
