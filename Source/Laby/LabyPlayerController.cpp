#include "LabyPlayerController.h"

#include "LabyPawn.h"


DEFINE_LOG_CATEGORY(LogLabyPlayerController)


ALabyPlayerController::ALabyPlayerController() {
	AutoReceiveInput = EAutoReceiveInput::Player0;
}

void ALabyPlayerController::PlayerTick(float DeltaTime) {
	Super::PlayerTick(DeltaTime);

	UE_LOG(LogLabyPlayerController, Verbose, TEXT("%S"), __FUNCTION__)

	ProcessMotionState();

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
		UE_LOG(LogLabyPlayerController, Warning, TEXT("%S: ControlledPawn is invalid!"), __FUNCTION__)
	}

	ResetInputs();
}

bool ALabyPlayerController::InputTouch(uint32 Handle, ETouchType::Type Type, const FVector2D& TouchLocation,
                                       float Force, FDateTime DeviceTimestamp, uint32 TouchpadIndex) {
	UE_LOG(LogLabyPlayerController, Verbose, TEXT("%S: Handle: %lu, Type: %s, TouchLocation: %s, TouchpadIndex: %lu"),
	       __FUNCTION__, Handle, *UEnum::GetValueAsString(Type), *TouchLocation.ToString(), TouchpadIndex)

	const auto bParentResult = Super::InputTouch(Handle, Type, TouchLocation, Force, DeviceTimestamp, TouchpadIndex);
	UE_LOG(LogLabyPlayerController, Verbose, TEXT("%S: Super::InputTouch: %s"), __FUNCTION__,
	       bParentResult ? TEXT("True") : TEXT("False"))

	if (!bParentResult) {
		return false;
	}

	switch (Type) {
	case ETouchType::Began:
		this->TouchLocation = TouchLocation;
		return true;
	case ETouchType::Ended:
		this->TouchLocation = FVector2D::ZeroVector;
		return true;
	case ETouchType::Moved:
		if (this->TouchLocation != FVector2D::ZeroVector) {
			static constexpr float TouchInputMultiplier = 0.1f;
			const auto InputAxis = (TouchLocation - this->TouchLocation) * TouchInputMultiplier;
			LookRight(InputAxis.X);
			LookDown(-InputAxis.Y);
			this->TouchLocation = TouchLocation;
		}
		return true;
	default:
		return false;
	}
}

void ALabyPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &ALabyPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ALabyPlayerController::MoveRight);
	InputComponent->BindAxis("LookRight", this, &ALabyPlayerController::LookRight);
	InputComponent->BindAxis("LookDown", this, &ALabyPlayerController::LookDown);
	InputComponent->BindAxis("Acceleration", this, &ALabyPlayerController::Acceleration);
}

void ALabyPlayerController::MoveForward(const float AxisValue) {
	UE_LOG(LogLabyPlayerController, Verbose, TEXT("%S: AxisValue: %f"), __FUNCTION__, AxisValue)
	ForwardInput += AxisValue;
}

void ALabyPlayerController::MoveRight(const float AxisValue) {
	UE_LOG(LogLabyPlayerController, Verbose, TEXT("%S: AxisValue: %f"), __FUNCTION__, AxisValue)
	RightInput += AxisValue;
}

void ALabyPlayerController::LookRight(const float AxisValue) {
	UE_LOG(LogLabyPlayerController, Verbose, TEXT("%S: AxisValue: %f"), __FUNCTION__, AxisValue)
	YawInput += AxisValue;
}

void ALabyPlayerController::LookDown(const float AxisValue) {
	UE_LOG(LogLabyPlayerController, Verbose, TEXT("%S: AxisValue: %f"), __FUNCTION__, AxisValue)
	PitchInput += AxisValue;
}

void ALabyPlayerController::Acceleration(const float AxisValue) {
	ForcePerSecFinal = (1.0f + (AccelerationMultiplier - 1.0f) * AxisValue) * ForcePerSecBase;
}

void ALabyPlayerController::ProcessMotionState() {
	FVector Tilt, RotationRate, Gravity, Acceleration;
	GetInputMotionState(Tilt, RotationRate, Gravity, Acceleration);

	UE_LOG(LogLabyPlayerController, Verbose, TEXT("%S: Tilt: %s"), __FUNCTION__, *Tilt.ToString())

	static constexpr float TouchMotionMultiplier = 1.f;
	MoveForward(Tilt.X * TouchMotionMultiplier);
	MoveRight(Tilt.Y * TouchMotionMultiplier);
}

void ALabyPlayerController::ResetInputs() {
	ForcePerSecFinal = 0.0f;
	ForwardInput = 0.0f;
	RightInput = 0.0f;
	YawInput = 0.0f;
	PitchInput = 0.0f;
}
