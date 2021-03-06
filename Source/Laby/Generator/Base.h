#pragma once

#include "Logging/LogMacros.h"
#include "LabyContainer.h"

DECLARE_LOG_CATEGORY_EXTERN(LogLabyGenerator, Log, All)

namespace Generator {

class Base {
public:
	Base();
	virtual ~Base();

	void Init(int32 NewMaxIterations);
	/** Generates maze */
	virtual LabyContainerPtr GenerateMaze(int32 HSize, int32 VSize);

protected:
	int32 MaxIterations{ 10000 };
};

}  // namespace Generator
