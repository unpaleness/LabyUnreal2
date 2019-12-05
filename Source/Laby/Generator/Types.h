#pragma once

#include "ObjectMacros.h"

UENUM()
enum class EGeneratorType : uint8 {
	OnlyWalls            UMETA(DisplayName = "OnlyWalls"),
	RecursiveBacktracker UMETA(DisplayName = "RecursiveBacktracker")
};
