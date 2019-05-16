#pragma once

#include "GeneratorBase.h"

class RecursiveBacktracking : public GeneratorBase {
public:
	RecursiveBacktracking();

	/** Generates maze using Recursive Backtracking algorithm */
	virtual void GenerateMaze() override;

private:
	/**
	 * Gets index of not visited neighbour cell
	 * @param Index                Index of current cell
	 * @param Cells                Array of cells, where "true" - visited, "false" - unvisited
	 * @return Index of not visited neighbour cell or -1 if where is no such cell
	 */
	int32 GetRandomNeighbour(int32 Index, bool* Cells);
	/**
	 * Tries to break a wall between two cells
	 * @param C1                   Index of first cell
	 * @param C2                   Index of second cell
	 */
	void BreakWall(int32 C1, int32 C2);
};
