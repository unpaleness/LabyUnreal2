// Fill out your copyright notice in the Description page of Project Settings.

#include "LabyPawn.h"
#include "ConstructorHelpers.h"

// Sets default values
ALabyPawn::ALabyPawn() {
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshSphere(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);
	if (MeshSphere.Succeeded()) {
		Mesh->SetStaticMesh(MeshSphere.Object);
	}
	Mesh->SetRelativeScale3D(FVector(0.5f));

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>("Movement");
}

// Called when the game starts or when spawned
void ALabyPawn::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void ALabyPawn::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ALabyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
