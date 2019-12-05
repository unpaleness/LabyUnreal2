#pragma once

#include <vector>
#include <memory>

#include <Platform.h>

struct LabyContainer;

/** true - wall exists, false - wall doesn't exist */
typedef std::vector<bool> Walls;
typedef std::shared_ptr<Walls> WallsPtr;
typedef std::shared_ptr<LabyContainer> LabyContainerPtr;

struct LabyContainer {
public:
	LabyContainer();
	LabyContainer(int32 NewHSize, int32 NewVSize, bool DefaultValue = true);
	LabyContainer(const LabyContainer& other);
	LabyContainer(LabyContainer&& other);
	~LabyContainer();

	operator bool() const { return HWalls && VWalls; }

	int32 GetHSize() const;
	int32 GetVSize() const;

	/** Array of horizontal walls */
	WallsPtr HWalls;
	/** Array of vertical walls */
	WallsPtr VWalls;

private:
	int32 HSize;
	int32 VSize;
};
