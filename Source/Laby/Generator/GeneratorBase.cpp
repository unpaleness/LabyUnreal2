#include "GeneratorBase.h"

DEFINE_LOG_CATEGORY(LogLabyGenerator)

GeneratorBase::GeneratorBase() {}
GeneratorBase::~GeneratorBase() {};

void GeneratorBase::Init(int32 NewMaxIterations) {
	MaxIterations = NewMaxIterations;
}

LabyContainerPtr GeneratorBase::GenerateMaze(int32 HSize, int32 VSize) {
	return std::make_shared<LabyContainer>(HSize, VSize);
}
