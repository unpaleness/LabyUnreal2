#include "LabyPlayerController.h"

#include "LabyPawn.h"


DEFINE_LOG_CATEGORY(LogLabyPlayerController)


ALabyPlayerController::ALabyPlayerController() {
	AutoReceiveInput = EAutoReceiveInput::Player0;
}

void ALabyPlayerController::PlayerTick(float DeltaTime) {
	Super::PlayerTick(DeltaTime);

	if (const auto* LabyPawn = Cast<ALabyPawn>(GetPawn())) {
		FRotator MovementRotator = FRotator::ZeroRotator;
		if (auto* LabyPawnCamera = LabyPawn->GetCamera()) {
			MovementRotator = LabyPawnCamera->GetRelativeRotation();
			MovementRotator.Pitch = FMath::Clamp(MovementRotator.Pitch + PitchInput * DeltaTime * LookSpeed, -89.0f, 89.0f);
			MovementRotator.Yaw += YawInput * DeltaTime * LookSpeed;
			LabyPawnCamera->SetRelativeRotation(MovementRotator);
		}
		auto MovementDirection = MovementRotator.RotateVector({ForwardInput, RightInput, 0.0f});
		MovementDirection.Normalize();
		LabyPawn->GetMesh()->AddForce(MovementDirection * DeltaTime * ForcePerSecFinal * LabyPawn->GetMass(), NAME_None);
	} else {
		UE_LOG(LogLabyPlayerController, Warning, TEXT("%S: ControlledPawn is invalid!"), __FUNCTION__);
	}

	ResetAllInput();
}

void ALabyPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &ALabyPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ALabyPlayerController::MoveRight);
	InputComponent->BindAxis("LookRight", this, &ALabyPlayerController::LookRight);
	InputComponent->BindAxis("LookDown", this, &ALabyPlayerController::LookDown);
	InputComponent->BindAxis("Acceleration", this, &ALabyPlayerController::Acceleration);

	InputComponent->BindTouch(IE_Pressed, this, &ALabyPlayerController::OnTouchBegin);
	InputComponent->BindTouch(IE_Released, this, &ALabyPlayerController::OnTouchEnd);
	InputComponent->BindTouch(IE_Repeat, this, &ALabyPlayerController::OnTouchMove);
}

void ALabyPlayerController::MoveForward(const float AxisValue) {
	UE_LOG(LogLabyPlayerController, Verbose, TEXT("%S: AxisValue: %f"), __FUNCTION__, AxisValue);
	ForwardInput = AxisValue;
}

void ALabyPlayerController::MoveRight(const float AxisValue) {
	UE_LOG(LogLabyPlayerController, Verbose, TEXT("%S: AxisValue: %f"), __FUNCTION__, AxisValue);
	RightInput = AxisValue;
}

void ALabyPlayerController::LookRight(const float AxisValue) {
	UE_LOG(LogLabyPlayerController, Verbose, TEXT("%S: AxisValue: %f"), __FUNCTION__, AxisValue);
	YawInput = AxisValue;
}

void ALabyPlayerController::LookDown(const float AxisValue) {
	UE_LOG(LogLabyPlayerController, Verbose, TEXT("%S: AxisValue: %f"), __FUNCTION__, AxisValue);
	PitchInput = AxisValue;
}

void ALabyPlayerController::Acceleration(const float AxisValue) {
	ForcePerSecFinal = (1.0f + (AccelerationMultiplier - 1.0f) * AxisValue) * ForcePerSecBase;
}

void ALabyPlayerController::OnTouchBegin(ETouchIndex::Type TouchIndex, FVector Location) {
	if (TouchIndex == ETouchIndex::Touch1) {
		LastTouchLocation = FVector2D(Location);
	}
}

void ALabyPlayerController::OnTouchEnd(ETouchIndex::Type TouchIndex, FVector Location) {
	if (TouchIndex == ETouchIndex::Touch1) {
		LastTouchLocation = FVector2D::ZeroVector;
	}
}

void ALabyPlayerController::OnTouchMove(ETouchIndex::Type TouchIndex, FVector Location) {
	if (TouchIndex == ETouchIndex::Touch1 && !LastTouchLocation.IsZero()) {
		static constexpr float TouchRotationScale = 0.1f;
		const auto DragDelta = (FVector2D(Location) - LastTouchLocation) * TouchRotationScale;

		LookRight(DragDelta.X);
		LookDown(-DragDelta.Y);

		LastTouchLocation = FVector2D(Location);
	}
}

void ALabyPlayerController::ResetAllInput() {
	ForcePerSecFinal = 0.f;
	ForwardInput = 0.f;
	RightInput = 0.f;
	YawInput = 0.f;
	PitchInput = 0.f;
}
