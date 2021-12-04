#include "LabyPawn.h"


ALabyPawn::ALabyPawn() {
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(Mesh);

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>("Movement");
}

void ALabyPawn::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ALabyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
