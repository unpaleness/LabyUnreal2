#include "OnlyWalls.h"

namespace Generator {

OnlyWalls::OnlyWalls() {}

LabyContainerPtr OnlyWalls::GenerateMaze(int32 HSize, int32 VSize) {
	return Base::GenerateMaze(HSize, VSize);
}

}  // namespace Generator
