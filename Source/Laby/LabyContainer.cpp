#include "LabyContainer.h"

LabyContainer::LabyContainer() {}

LabyContainer::LabyContainer(int32 NewHSize, int32 NewVSize, bool DefaultValue) {
	HSize = NewHSize;
	VSize = NewVSize;
	HWalls = std::make_shared<Walls>((VSize + 1) * HSize, DefaultValue);
	VWalls = std::make_shared<Walls>(VSize * (HSize + 1), DefaultValue);
}

LabyContainer::LabyContainer(const LabyContainer& other) :
	HWalls(other.HWalls), VWalls(other.VWalls) {}

LabyContainer::LabyContainer(LabyContainer&& other) :
	HWalls(std::move(other.HWalls)), VWalls(std::move(other.VWalls)) {}

LabyContainer::~LabyContainer() {}

int32 LabyContainer::GetHSize() const { return HSize; }
int32 LabyContainer::GetVSize() const { return VSize; }
