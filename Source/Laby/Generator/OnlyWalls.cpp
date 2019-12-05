#include "OnlyWalls.h"

OnlyWalls::OnlyWalls() {}

LabyContainerPtr OnlyWalls::GenerateMaze(int32 HSize, int32 VSize) {
	return GeneratorBase::GenerateMaze(HSize, VSize);
}
