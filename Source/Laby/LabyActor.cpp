#include "LabyActor.h"

#include "Generator/Factory.h"

DEFINE_LOG_CATEGORY(LogLabyActor)

ALabyActor::ALabyActor() {
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>("Mesh");
	Mesh->SetGenerateOverlapEvents(false);
	Mesh->bUseAsyncCooking = true;

	SetRootComponent(Mesh);
}

void ALabyActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ALabyActor::OnConstruction(const FTransform& Transform) {
	if (HasChanges()) {
		GenerateMesh();
	}
}

void ALabyActor::BeginPlay() {
	Super::BeginPlay();

	IsMazeNeedGenerate = true;
	GenerateMesh();
}

void ALabyActor::GenerateMesh() {
	S = CellSize;

	InitArrays();

	Mesh->ClearMeshSection(0);
	Mesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);
}

bool ALabyActor::HasChanges() {
	bool result{ false };
	if (CellSize != S) {
		result = true;
	}
	if (HCells != X || VCells != Y || Algorithm != Alg) {
		IsMazeNeedGenerate = true;
		result = true;
	}
	if (WallWidthRelative != W || Height != H) {
		result = true;
	}
	return result;
}

void ALabyActor::InitArrays() {
	Vertices.Empty();
	Triangles.Empty();
	Normals.Empty();
	UVs.Empty();

	if (IsMazeNeedGenerate) {
		GenerateMaze();
	}
	W = WallWidthRelative;
	H = Height;

	// Add floor
	const auto RadiusX = S * (W * (X + 1) + X) / 2.0f;
	const auto RadiusY = S * (W * (Y + 1) + Y) / 2.0f;
	AddPlane(FVector{-RadiusX, -RadiusY, 0.0f}, FVector{RadiusX, -RadiusY, 0.0f},
	         FVector{-RadiusX, RadiusY, 0.0f}, FVector{RadiusX, RadiusY, 0.0f});

	for (int32 j = 0; j < Y + 1; ++j) {
		for (int32 i = 0; i < X + 1; ++i) {
			// Vertical walls
			if (j < Y) {
				if (Maze->VWalls->at(j * (X + 1) + i)) {
					AddCuboid(
						FVector(
							S * ((W + 1.0f) * i - ((W + 1.0f) * X + W) / 2.0f),
							S * (W + (W + 1.0f) * j - ((W + 1.0f) * Y + W) / 2.0f),
							0.0f
						),
						FVector(
							S * (W + (W + 1.0f) * i - ((W + 1.0f) * X + W) / 2.0f),
							S * ((W + 1.0f) * (j + 1) - ((W + 1.0f) * Y + W) / 2.0f),
							H
						), EMazeCubiodFaces::Horizontal
					);
				}
			}
			// Horizontal walls
			if (i < X) {
				if (Maze->HWalls->at(j * X + i)) {
					AddCuboid(
						FVector(
							S * (W + (W + 1.0f) * i - ((W + 1.0f) * X + W) / 2.0f),
							S * ((W + 1.0f) * j - ((W + 1.0f) * Y + W) / 2.0f),
							0.0f
						),
						FVector(
							S * ((W + 1.0f) * (i + 1) - ((W + 1.0f) * X + W) / 2.0f),
							S * (W + (W + 1.0f) * j - ((W + 1.0f) * Y + W) / 2.0f),
							H
						), EMazeCubiodFaces::Vertical
					);
				}
			}
			// Corner
			AddCuboid(
				FVector(
					S * ((W + 1.0f) * i - ((W + 1.0f) * X + W) / 2.0f),
					S * ((W + 1.0f) * j - ((W + 1.0f) * Y + W) / 2.0f),
					0.0f
				),
				FVector(
					S * (W + (W + 1.0f) * i - ((W + 1.0f) * X + W) / 2.0f),
					S * (W + (W + 1.0f) * j - ((W + 1.0f) * Y + W) / 2.0f),
					1.1f * H
				), EMazeCubiodFaces::All
			);
		}
	}
}

void ALabyActor::AddPlane(FVector P1, FVector P2, FVector P3, FVector P4) {
	int32 V{ Vertices.Num() };

	Vertices.Add(P1);
	Vertices.Add(P2);
	Vertices.Add(P3);
	Vertices.Add(P4);

	Triangles.Add(V + 0);
	Triangles.Add(V + 3);
	Triangles.Add(V + 1);

	Triangles.Add(V + 0);
	Triangles.Add(V + 2);
	Triangles.Add(V + 3);

	UVs.Add(FVector2D(0.0f, 1.0f));
	UVs.Add(FVector2D(0.0f, 0.0f));
	UVs.Add(FVector2D(1.0f, 1.0f));
	UVs.Add(FVector2D(1.0f, 0.0f));

	auto Normal = FVector::CrossProduct(P2 - P1, P3 - P1);
	Normal.Normalize();
	Normals.Append({Normal, Normal, Normal, Normal});
}

void ALabyActor::AddCuboid(FVector P1, FVector P2, EMazeCubiodFaces Direction) {
	auto AddHorizontalPlanes = [this, P1, P2]() {
		AddPlane(FVector{P1.X, P2.Y, P1.Z}, FVector{P1.X, P1.Y, P1.Z}, FVector{P1.X, P2.Y, P2.Z}, FVector{P1.X, P1.Y, P2.Z});
		AddPlane(FVector{P2.X, P2.Y, P1.Z}, FVector{P2.X, P2.Y, P2.Z}, FVector{P2.X, P1.Y, P1.Z}, FVector{P2.X, P1.Y, P2.Z});
	};

	auto AddVerticalPlanes = [this, P1, P2]() {
		AddPlane(FVector{P1.X, P1.Y, P1.Z}, FVector{P2.X, P1.Y, P1.Z}, FVector{P1.X, P1.Y, P2.Z}, FVector{P2.X, P1.Y, P2.Z});
		AddPlane(FVector{P1.X, P2.Y, P1.Z}, FVector{P1.X, P2.Y, P2.Z}, FVector{P2.X, P2.Y, P1.Z}, FVector{P2.X, P2.Y, P2.Z});
	};

	switch (Direction) {
	case EMazeCubiodFaces::Horizontal:
		AddHorizontalPlanes();
		break;
	case EMazeCubiodFaces::Vertical:
		AddVerticalPlanes();
		break;
	default:
		AddHorizontalPlanes();
		AddVerticalPlanes();
		break;
	}

	// Top Plane
	AddPlane(FVector{P1.X, P1.Y, P2.Z}, FVector{P2.X, P1.Y, P2.Z}, FVector{P1.X, P2.Y, P2.Z}, FVector{P2.X, P2.Y, P2.Z});
}

void ALabyActor::GenerateMaze() {
	X = HCells;
	Y = VCells;
	Alg = Algorithm;

	auto Generator = Generator::Create(Alg);

	if (Generator) {
		Generator->Init(MaxIterations);
		Maze = Generator->GenerateMaze(X, Y);
		IsMazeNeedGenerate = false;
	} else {
		UE_LOG(LogLabyActor, Error, TEXT("Invalid Generator::Type value: %i, cannot create maze generator"), static_cast<int32>(Alg))
	}
}
