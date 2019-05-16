#include "RecursiveBacktracking.h"

RecursiveBacktracking::RecursiveBacktracking() {}

void RecursiveBacktracking::GenerateMaze() {
	HWalls->Empty();
	VWalls->Empty();

	// Init all walls with
	HWalls->Init(true, (Y + 1) * X);
	VWalls->Init(true, Y * (X + 1));

	// Temporary cell state:
	//   false - not visited
	//   true  - visited
	bool* Cells = new bool[X * Y];
	for (int32 i = 0; i < X * Y; ++i) {
		Cells[i] = false;
	}
	// Path of generator
	int32* Path = new int32[X * Y];
	// Elements in path
	int32 N{ 1 };

	// Start with random cell
	Path[0] = rand() % (X * Y);
	Cells[Path[0]] = true;

	int32 it{ 0 };
	while (N > 0) {
		if (++it > MaxIterations) {
			UE_LOG(LogLabyGenerator, Warning, TEXT("Limit of %i iteration exceeded!"), MaxIterations);
			break;
		}
		//UE_LOG(LogLabyGenerator, Display, TEXT("N: %i, Processing (%i, %i)"), N, Path[N - 1] % X, Path[N - 1] / X);
		int32 NextCell{ GetRandomNeighbour(Path[N - 1], Cells) };
		if (NextCell >= X * Y) {
			UE_LOG(LogLabyGenerator, Warning, TEXT("Next cell index (%i) is above maximum possible (%i)!"), NextCell, X * Y - 1);
			break;
		}
		if (NextCell >= 0) {
			Path[N] = NextCell;
			Cells[Path[N]] = true;
			BreakWall(Path[N - 1], Path[N]);
			++N;
		}
		else {
			--N;
		}
	}

	//// Fill walls
	//for (int32 j = 0; j < Y + 1; ++j) {
	//	for (int32 i = 0; i < X + 1; ++i) {
	//		if (j < Y) {
	//			VWalls->Add(rand() % 2 ? true : false);
	//		}
	//		if (i < X) {
	//			HWalls->Add(rand() % 2 ? true : false);
	//		}
	//	}
	//}

	delete[] Path;
	delete[] Cells;
}

int32 RecursiveBacktracking::GetRandomNeighbour(int32 Index, bool* Cells) {
	// 0 - South (-X), 1 - North (+X), 2 - East(-1), 3 - West(+1)
	bool AvailDirs[4] = { false, false, false, false };
	uint8_t Dirs{ 0 };
	// Southern edge
	if (Index >= X && !Cells[Index - X]) {
		AvailDirs[0] = true;
		++Dirs;
	}
	// Northern edge
	if (Index < (Y - 1) * X && !Cells[Index + X]) {
		AvailDirs[1] = true;
		++Dirs;
	}
	// Eastern edge
	if (Index % X != 0 && !Cells[Index - 1]) {
		AvailDirs[2] = true;
		++Dirs;
	}
	// Western edge
	if (Index % X != X - 1 && !Cells[Index + 1]) {
		AvailDirs[3] = true;
		++Dirs;
	}
	//UE_LOG(LogLabyGenerator, Display, TEXT("Neighbours %i: %i, %i, %i, %i"), Dirs, AvailDirs[0], AvailDirs[1], AvailDirs[2], AvailDirs[3]);
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
				//UE_LOG(LogLabyGenerator, Display, TEXT("Southern dir (%i, %i)"), (Index - X) % X, (Index - X) / X);
				return Index - X;
			case 1:
				//UE_LOG(LogLabyGenerator, Display, TEXT("Northern dir (%i, %i)"), (Index + X) % X, (Index + X) / X);
				return Index + X;
			case 2:
				//UE_LOG(LogLabyGenerator, Display, TEXT("Eastern dir (%i, %i)"), (Index - 1) % X, (Index - 1) / X);
				return Index - 1;
			case 3:
				//UE_LOG(LogLabyGenerator, Display, TEXT("Western dir (%i, %i)"), (Index + 1) % X, (Index + 1) / X);
				return Index + 1;
			}
			break;
		}
	}
	return -1;
}

void RecursiveBacktracking::BreakWall(int32 C1, int32 C2) {
	//UE_LOG(LogLabyGenerator, Display, TEXT("Breaking wall (%i,%i)|(%i,%i)"), C1 % X, C1 / X, C2 % X, C2 / X);
	if (C1 < 0 || C2 < 0 || C1 >= X * Y || C2 >= X * Y) {
		return;
	}
	if (abs(C1 - C2) == 1 && C1 / X == C2 / X) {
		(*VWalls)[C1 / X * (X + 1) + (C1 > C2 ? C1 % X : C2 % X)] = false;
		return;
	}
	if (abs(C1 - C2) == X) {
		(*HWalls)[(C1 > C2 ? C1 : C2)] = false;
		return;
	}
	UE_LOG(LogLabyGenerator, Warning, TEXT("Breaking wall (%i,%i)|(%i,%i) failed!"), C1 % X, C1 / X, C2 % X, C2 / X);
}
