// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LabyPlayerController.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogLabyPlayerController, Log, All)

/**
 *
 */
UCLASS()
class LABY_API ALabyPlayerController : public APlayerController {
	GENERATED_BODY()

public:
	ALabyPlayerController();
	virtual void PlayerTick(float DeltaTime) override;

protected:
	virtual void SetupInputComponent() override;

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookRight(float AxisValue);
	void LookDown(float AxisValue);
	void Acceleration(float AxisValue);

protected:
	/** Movement speed without acceleration */
	UPROPERTY(EditAnywhere, Category = "Player")
	float MovementSpeed = 30.0f;
	/** Lookaround speed */
	UPROPERTY(EditAnywhere, Category = "Player")
	float LookSpeed = 0.75f;
	/** Relative speed when acceleration button is pressed */
	UPROPERTY(EditAnywhere, Category = "Player")
	float AccelerationMultiplier = 2.0f;
	/** Total movement speed including potential acceletation */
	UPROPERTY(EditAnywhere, Category = "Player")
	float FinalMovementSpeed = 0.0f;

	/** Unit or null vector which indicates whether player is moving forward/backward or not */
	FVector ForwardDirection = FVector(0.0f);

	/** Unit or null vector which indicates whether player is moving right/left or not */
	FVector RightDirection = FVector(0.0f);
	
};
