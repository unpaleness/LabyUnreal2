// Fill out your copyright notice in the Description page of Project Settings.

#include "LabyPlayerController.h"


DEFINE_LOG_CATEGORY(LogLabyPlayerController)


ALabyPlayerController::ALabyPlayerController() {
	AutoReceiveInput = EAutoReceiveInput::Player0;
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
	FVector NoZ(Forward.X, Forward.Y, 0.0f);
	NoZ.Normalize();
	GetPawn()->AddMovementInput(NoZ, AxisValue * FinalMovementSpeed);
}

void ALabyPlayerController::MoveRight(float AxisValue) {
}

void ALabyPlayerController::LookRight(float AxisValue) {
}

void ALabyPlayerController::LookDown(float AxisValue) {
}

void ALabyPlayerController::Acceleration(float AxisValue) {
	FinalMovementSpeed = (1.0f + (AccelerationMultiplier - 1.0f) * AxisValue) * MovementSpeed;
}
