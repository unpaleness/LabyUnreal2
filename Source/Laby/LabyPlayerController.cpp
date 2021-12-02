#include "LabyPlayerController.h"

#include "LabyPawn.h"


DEFINE_LOG_CATEGORY(LogLabyPlayerController)


ALabyPlayerController::ALabyPlayerController() {
	AutoReceiveInput = EAutoReceiveInput::Player0;
}

void ALabyPlayerController::PlayerTick(float DeltaTime) {
	Super::PlayerTick(DeltaTime);

	FVector MovementDirection(ForwardDirection + RightDirection);
	MovementDirection.Normalize();

	if (auto* LabyPawn = Cast<ALabyPawn>(GetPawn())) {
		LabyPawn->AddActorLocalOffset(MovementDirection * DeltaTime * FinalMovementSpeed);
		LabyPawn->AddActorLocalRotation(FRotator{0.0f, YawInput * DeltaTime * LookSpeed, 0.0f});
		if (auto* LabyPawnCamera = LabyPawn->GetCamera()) {
			auto NewRotation = LabyPawnCamera->GetRelativeRotation();
			NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + PitchInput * DeltaTime * LookSpeed, -89.0f, 89.0f);
			LabyPawnCamera->SetRelativeRotation(NewRotation);
		}
	} else {
		UE_LOG(LogLabyPlayerController, Warning, TEXT("ControlledPawn is invalid!"));
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
	const auto Forward = GetControlRotation().Vector();
	ForwardDirection = FVector(Forward.X, Forward.Y, 0.0f);
	ForwardDirection.Normalize();
	ForwardDirection *= AxisValue;
}

void ALabyPlayerController::MoveRight(const float AxisValue) {
	const auto Right = GetControlRotation().Quaternion().GetRightVector();
	RightDirection = FVector(Right.X, Right.Y, 0.0f);
	RightDirection.Normalize();
	RightDirection *= AxisValue;
}

void ALabyPlayerController::LookRight(const float AxisValue) {
	YawInput = AxisValue;
}

void ALabyPlayerController::LookDown(const float AxisValue) {
	PitchInput = AxisValue;
}

void ALabyPlayerController::Acceleration(const float AxisValue) {
	FinalMovementSpeed = (1.0f + (AccelerationMultiplier - 1.0f) * AxisValue) * MovementSpeed;
}
