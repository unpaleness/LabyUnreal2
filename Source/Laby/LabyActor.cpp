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
	Mesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, TArray<FVector>(), UVs, TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);
}

void ALabyActor::AddTriangle(int32 V1, int32 V2, int32 V3) {
	Triangles.Add(V1);
	Triangles.Add(V2);
	Triangles.Add(V3);
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
	UVs.Empty();
	
	if (IsMazeNeedGenerate) {
		GenerateMaze();
	}
	W = WallWidthRelative;
	H = Height;

	// Add floor
	AddPlane(
		FVector(
			-S * (W * (X + 1) + X) / 2.0f,
			-S * (W * (Y + 1) + Y) / 2.0f,
			0.0f
		),
		FVector(
			S * (W * (X + 1) + X) / 2.0f,
			S * (W * (Y + 1) + Y) / 2.0f,
			0.0f
		)
	);

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

void ALabyActor::AddPlane(FVector P1, FVector P2) {
	int32 V{ Vertices.Num() };

	FVector V0(P1);
	FVector V1(P2.X, P1.Y, (P1.Z + P2.Z) / 2.0f);
	FVector V2(P1.X, P2.Y, (P1.Z + P2.Z) / 2.0f);
	FVector V3(P2);

	Vertices.Add(V0);
	Vertices.Add(V1);
	Vertices.Add(V2);
	Vertices.Add(V3);

	AddTriangle(V + 0, V + 3, V + 1);
	AddTriangle(V + 0, V + 2, V + 3);

	UVs.Add(FVector2D(0.0f, 1.0f));
	UVs.Add(FVector2D(0.0f, 0.0f));
	UVs.Add(FVector2D(1.0f, 1.0f));
	UVs.Add(FVector2D(1.0f, 0.0f));
}

void ALabyActor::AddCuboid(FVector P1, FVector P2, EMazeCubiodFaces Direction) {
	int32 V{ Vertices.Num() };

	FVector V0(P1);
	FVector V1(P1.X, P1.Y, P2.Z);
	FVector V2(P1.X, P2.Y, P1.Z);
	FVector V3(P1.X, P2.Y, P2.Z);
	FVector V4(P2.X, P1.Y, P1.Z);
	FVector V5(P2.X, P1.Y, P2.Z);
	FVector V6(P2.X, P2.Y, P1.Z);
	FVector V7(P2);

	auto AddHorizontalVertices = [=]() {
		Vertices.Add(V0);
		Vertices.Add(V1);
		Vertices.Add(V2);
		Vertices.Add(V3);
		Vertices.Add(V6);
		Vertices.Add(V7);
		Vertices.Add(V4);
		Vertices.Add(V5);
	};

	auto AddVerticalVertices = [=]() {
		Vertices.Add(V2);
		Vertices.Add(V3);
		Vertices.Add(V6);
		Vertices.Add(V7);
		Vertices.Add(V4);
		Vertices.Add(V5);
		Vertices.Add(V0);
		Vertices.Add(V1);
	};

	switch (Direction) {
	case EMazeCubiodFaces::Horizontal:
		AddHorizontalVertices();
		break;
	case EMazeCubiodFaces::Vertical:
		AddVerticalVertices();
		break;
	default:
		AddHorizontalVertices();
		AddVerticalVertices();
		break;
	}
	Vertices.Add(V3);
	Vertices.Add(V1);
	Vertices.Add(V7);
	Vertices.Add(V5);

	for (int32 i = 0; i < (Vertices.Num() - V) / 4; ++i) {
		AddTriangle(V + i * 4, V + i * 4 + 3, V + i * 4 + 1);
		AddTriangle(V + i * 4, V + i * 4 + 2, V + i * 4 + 3);
		UVs.Add(FVector2D(0.0f, 1.0f));
		UVs.Add(FVector2D(0.0f, 0.0f));
		UVs.Add(FVector2D(1.0f, 1.0f));
		UVs.Add(FVector2D(1.0f, 0.0f));
	}
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
