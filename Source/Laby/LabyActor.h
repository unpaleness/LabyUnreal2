#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "LabyContainer.h"
#include "Generator/Types.h"
#include "LabyActor.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogLabyActor, Log, All)

enum class EMazeCubiodFaces : uint8 {
	All,
	Vertical,
	Horizontal
};

UCLASS()
class LABY_API ALabyActor : public AActor {
	GENERATED_BODY()

public:
	ALabyActor();
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;

private:
	void GenerateMesh();
	bool HasChanges();
	void InitArrays();
	void AddPlane(FVector P1, FVector P2, FVector P3, FVector P4);
	void AddCuboid(FVector P1, FVector P2, EMazeCubiodFaces Direction);
	void GenerateMaze();

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	UProceduralMeshComponent* Mesh;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Maze")
	UMaterialInterface* Material;

	/** Number of cells in horizontal direction */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Maze", meta = (ClampMin = "1", ClampMax = "256", UIMin = "1", UIMax = "256"))
	int32 HCells = 1;

	/** Number of cells in vertical direction */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Maze", meta = (ClampMin = "1", ClampMax = "256", UIMin = "1", UIMax = "256"))
	int32 VCells = 1;

	/** Size of one cell */
	UPROPERTY(BlueprintReadonly, EditAnywhere, Category = "Maze")
	float CellSize = 100.0f;

	/** Width of labyrinth wall in relative units comparing with cell's size*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Maze", meta = (ClampMin = "0"))
	float WallWidthRelative = 0.05f;

	/** Wall height */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Maze", meta = (ClampMin = "0"))
	float Height = 50.0f;

	/** Maze generator algorithm */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Maze")
	EGeneratorType Algorithm = EGeneratorType::OnlyWalls;

	/** Interation limit for generator */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Maze", meta = (ClampMin = "0"))
	int32 MaxIterations = 100000;

private:
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	/** Inner cell size */
	float S = 0.0f;
	/** Inner number of cells in horizontal direction */
	int32 X;
	/** Inner number of cells in vertical direction */
	int32 Y;
	/** Inner WallWidthRelative */
	float W;
	/** Inner Height */
	float H;
	/** A pointer to struct with walls */
	LabyContainerPtr Maze;
	/** Whether walls need to generate */
	bool IsMazeNeedGenerate = true;
	/** Inner algorithm */
	EGeneratorType Alg;
};
