#include "LabyPlayerController.h"

#include "LabyPawn.h"


DEFINE_LOG_CATEGORY(LogLabyPlayerController)


ALabyPlayerController::ALabyPlayerController() {
	AutoReceiveInput = EAutoReceiveInput::Player0;
}

void ALabyPlayerController::PlayerTick(float DeltaTime) {
	Super::PlayerTick(DeltaTime);

	if (auto* LabyPawn = Cast<ALabyPawn>(GetPawn())) {
		LabyPawn->AddActorLocalRotation(FRotator{0.0f, YawInput * DeltaTime * LookSpeed, 0.0f});
		auto MovementDirection = LabyPawn->GetActorRotation().RotateVector(FVector{ForwardInput, RightInput, 0.0f});
		MovementDirection.Normalize();
		LabyPawn->GetMesh()->AddForce(MovementDirection * DeltaTime * ForcePerSecFinal * LabyPawn->GetMass(), NAME_None);
		if (auto* LabyPawnCamera = LabyPawn->GetCamera()) {
			auto NewRotation = LabyPawnCamera->GetRelativeRotation();
			NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + PitchInput * DeltaTime * LookSpeed, -89.0f, 89.0f);
			LabyPawnCamera->SetRelativeRotation(NewRotation);
		}
	} else {
		UE_LOG(LogLabyPlayerController, Warning, TEXT("%S: ControlledPawn is invalid!"), __FUNCTION__);
	}
}

void ALabyPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	this->InputComponent->BindAxis("MoveForward", this, &ALabyPlayerController::MoveForward);
	this->InputComponent->BindAxis("MoveRight", this, &ALabyPlayerController::MoveRight);
	this->InputComponent->BindAxis("LookRight", this, &ALabyPlayerController::LookRight);
	this->InputComponent->BindAxis("LookDown", this, &ALabyPlayerController::LookDown);
	this->InputComponent->BindAxis("Acceleration", this, &ALabyPlayerController::Acceleration);
}

void ALabyPlayerController::MoveForward(const float AxisValue) {
	ForwardInput = AxisValue;
}

void ALabyPlayerController::MoveRight(const float AxisValue) {
	RightInput = AxisValue;
}

void ALabyPlayerController::LookRight(const float AxisValue) {
	YawInput = AxisValue;
}

void ALabyPlayerController::LookDown(const float AxisValue) {
	PitchInput = AxisValue;
}

void ALabyPlayerController::Acceleration(const float AxisValue) {
	ForcePerSecFinal = (1.0f + (AccelerationMultiplier - 1.0f) * AxisValue) * ForcePerSecBase;
}
