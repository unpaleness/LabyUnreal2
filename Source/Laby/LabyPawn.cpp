#include "LabyPawn.h"


ALabyPawn::ALabyPawn() {
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(Mesh);

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>("Movement");
}

void ALabyPawn::BeginPlay() {
	Super::BeginPlay();

	if (PPOutlineMaterial) {
		PPOutlineMaterialDynamic = UMaterialInstanceDynamic::Create(PPOutlineMaterial, this);
		Camera->AddOrUpdateBlendable(PPOutlineMaterialDynamic);
	}
}

void ALabyPawn::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ALabyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
