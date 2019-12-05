#include "Base.h"

DEFINE_LOG_CATEGORY(LogLabyGenerator)

namespace Generator {

Base::Base() {}
Base::~Base() {};

void Base::Init(int32 NewMaxIterations) {
	MaxIterations = NewMaxIterations;
}

LabyContainerPtr Base::GenerateMaze(int32 HSize, int32 VSize) {
	return std::make_shared<LabyContainer>(HSize, VSize);
}

}  // namespace Generator
