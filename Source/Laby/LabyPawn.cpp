#include "LabyPawn.h"

#include "Engine/CollisionProfile.h"


DEFINE_LOG_CATEGORY(LogLabyPawn)


ALabyPawn::ALabyPawn() {
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(RootComponent);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	Mesh->SetSimulatePhysics(true);

	UE_LOG(LogLabyPawn, Verbose, TEXT("%S: Mass: %f"), __FUNCTION__, Mesh->GetBodyInstance()->GetBodyMass())
}

void ALabyPawn::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	RootComponent->SetWorldLocation(Mesh->GetComponentLocation());
}

void ALabyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ALabyPawn::BeginPlay() {
	Super::BeginPlay();

	Mesh->SetWorldLocation(RootComponent->GetComponentLocation());
}
