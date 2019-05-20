// Fill out your copyright notice in the Description page of Project Settings.

#include "LabyPlayerController.h"


DEFINE_LOG_CATEGORY(LogLabyPlayerController)


ALabyPlayerController::ALabyPlayerController() {
	AutoReceiveInput = EAutoReceiveInput::Player0;
}

void ALabyPlayerController::PlayerTick(float DeltaTime) {
	Super::PlayerTick(DeltaTime);

	FVector MovementDirection(ForwardDirection + RightDirection);
	MovementDirection.Normalize();
	GetPawn()->AddMovementInput(MovementDirection, DeltaTime * FinalMovementSpeed);
}

void ALabyPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	this->InputComponent->BindAxis("MoveForward", this, &ALabyPlayerController::MoveForward);
	this->InputComponent->BindAxis("MoveRight", this, &ALabyPlayerController::MoveRight);
	this->InputComponent->BindAxis("LookRight", this, &ALabyPlayerController::LookRight);
	this->InputComponent->BindAxis("LookDown", this, &ALabyPlayerController::LookDown);
	this->InputComponent->BindAxis("Acceleration", this, &ALabyPlayerController::Acceleration);
}

void ALabyPlayerController::MoveForward(float AxisValue) {
	FVector Forward = GetControlRotation().Vector();
	ForwardDirection = FVector(Forward.X, Forward.Y, 0.0f);
	ForwardDirection.Normalize();
	ForwardDirection *= AxisValue;
}

void ALabyPlayerController::MoveRight(float AxisValue) {
	FVector Right = GetControlRotation().Quaternion().GetRightVector();
	RightDirection = FVector(Right.X, Right.Y, 0.0f);
	RightDirection.Normalize();
	RightDirection *= AxisValue;
}

void ALabyPlayerController::LookRight(float AxisValue) {
	AddYawInput(AxisValue * LookSpeed);
}

void ALabyPlayerController::LookDown(float AxisValue) {
	AddPitchInput(-AxisValue * LookSpeed);
}

void ALabyPlayerController::Acceleration(float AxisValue) {
	FinalMovementSpeed = (1.0f + (AccelerationMultiplier - 1.0f) * AxisValue) * MovementSpeed;
}
