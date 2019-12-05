#include "RecursiveBacktracking.h"

namespace {

int32 GetRandomNeighbour(int32 Index, const std::vector<bool>& Cells, const LabyContainerPtr& Maze) {
	if (!Maze || !(*Maze)) {
		UE_LOG(LogLabyGenerator, Error, TEXT("Maze is uninitialized!"))
			return -1;
	}

	const auto& HSize = Maze->GetHSize();
	const auto& VSize = Maze->GetVSize();

	// 0 - South (-HSize), 1 - North (+HSize), 2 - East(-1), 3 - West(+1)
	bool AvailDirs[4] = { false, false, false, false };
	uint8_t Dirs{ 0 };
	// Southern edge
	if (Index >= HSize && !Cells[Index - HSize]) {
		AvailDirs[0] = true;
		++Dirs;
	}
	// Northern edge
	if (Index < (VSize - 1) * HSize && !Cells[Index + HSize]) {
		AvailDirs[1] = true;
		++Dirs;
	}
	// Eastern edge
	if (Index % HSize != 0 && !Cells[Index - 1]) {
		AvailDirs[2] = true;
		++Dirs;
	}
	// Western edge
	if (Index % HSize != HSize - 1 && !Cells[Index + 1]) {
		AvailDirs[3] = true;
		++Dirs;
	}
	//UE_LOG(LogLabyGenerator, Display, TEXT("Neighbours %i: %i, %i, %i, %i"), Dirs, AvailDirs[0], AvailDirs[1], AvailDirs[2], AvailDirs[3])
	// No neighbours
	if (Dirs <= 0) {
		return -1;
	}
	uint8_t Dir = (rand() % Dirs) + 1;
	uint8_t ValidDir{ 0 };
	for (uint8_t i = 0; i < 4; ++i) {
		ValidDir += (AvailDirs[i] ? 1 : 0);
		if (ValidDir == Dir) {
			switch (i) {
			case 0:
				//UE_LOG(LogLabyGenerator, Display, TEXT("Southern dir (%i, %i)"), (Index - HSize) % HSize, (Index - HSize) / HSize)
				return Index - HSize;
			case 1:
				//UE_LOG(LogLabyGenerator, Display, TEXT("Northern dir (%i, %i)"), (Index + HSize) % HSize, (Index + HSize) / HSize)
				return Index + HSize;
			case 2:
				//UE_LOG(LogLabyGenerator, Display, TEXT("Eastern dir (%i, %i)"), (Index - 1) % HSize, (Index - 1) / HSize)
				return Index - 1;
			case 3:
				//UE_LOG(LogLabyGenerator, Display, TEXT("Western dir (%i, %i)"), (Index + 1) % HSize, (Index + 1) / HSize)
				return Index + 1;
			}
			break;
		}
	}
	return -1;
}

void BreakWall(int32 C1, int32 C2, LabyContainerPtr Maze) {
	if (!Maze || !(*Maze)) {
		UE_LOG(LogLabyGenerator, Error, TEXT("Maze is uninitialized!"))
			return;
	}

	const auto& HSize = Maze->GetHSize();
	const auto& VSize = Maze->GetVSize();

	//UE_LOG(LogLabyGenerator, Display, TEXT("Breaking wall (%i,%i)|(%i,%i)"), C1 % HSize, C1 / HSize, C2 % HSize, C2 / HSize)
	if (C1 < 0 || C2 < 0 || C1 >= HSize * VSize || C2 >= HSize * VSize) {
		return;
	}
	if (abs(C1 - C2) == 1 && C1 / HSize == C2 / HSize) {
		Maze->VWalls->at(C1 / HSize * (HSize + 1) + (C1 > C2 ? C1 % HSize : C2 % HSize)) = false;
		return;
	}
	if (abs(C1 - C2) == HSize) {
		Maze->HWalls->at(C1 > C2 ? C1 : C2) = false;
		return;
	}
	UE_LOG(LogLabyGenerator, Warning, TEXT("Breaking wall (%i,%i)|(%i,%i) failed!"), C1 % HSize, C1 / HSize, C2 % HSize, C2 / HSize)
}

}  // namespace

RecursiveBacktracking::RecursiveBacktracking() {}

LabyContainerPtr RecursiveBacktracking::GenerateMaze(int32 NewHSize, int32 NewVSize) {
	auto Maze = GeneratorBase::GenerateMaze(NewHSize, NewVSize);

	if (!Maze || !(*Maze)) {
		UE_LOG(LogLabyGenerator, Error, TEXT("Maze is uninitialized!"))
		return nullptr;
	}

	const auto& HSize = Maze->GetHSize();
	const auto& VSize = Maze->GetVSize();

	// Temporary cell state:
	//   false - not visited
	//   true  - visited
	std::vector<bool> Cells(HSize * VSize, false);
	// Path of generator
	std::vector<int32> Path(HSize * VSize);
	// Elements in path
	int32 N{ 1 };

	// Start with random cell
	Path[0] = rand() % (HSize * VSize);
	Cells[Path[0]] = true;

	int32 it{ 0 };
	while (N > 0) {
		if (++it > MaxIterations) {
			UE_LOG(LogLabyGenerator, Warning, TEXT("Limit of %i iteration exceeded!"), MaxIterations)
			break;
		}
		//UE_LOG(LogLabyGenerator, Display, TEXT("N: %i, Processing (%i, %i)"), N, Path[N - 1] % HSize, Path[N - 1] / HSize)
		int32 NextCell{ GetRandomNeighbour(Path[N - 1], Cells, Maze) };
		if (NextCell >= HSize * VSize) {
			UE_LOG(LogLabyGenerator, Warning, TEXT("Next cell index (%i) is above maximum possible (%i)!"), NextCell, HSize * VSize - 1)
			break;
		}
		if (NextCell >= 0) {
			Path[N] = NextCell;
			Cells[Path[N]] = true;
			BreakWall(Path[N - 1], Path[N], Maze);
			++N;
		}
		else {
			--N;
		}
	}

	return Maze;
}
