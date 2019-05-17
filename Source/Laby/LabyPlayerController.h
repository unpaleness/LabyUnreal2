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

protected:
	virtual void SetupInputComponent() override;

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookRight(float AxisValue);
	void LookDown(float AxisValue);
	void Acceleration(float AxisValue);

protected:
	UPROPERTY(EditAnywhere, Category = "Player")
	float MovementSpeed = 0.25f;
	UPROPERTY(EditAnywhere, Category = "Player")
	float LookSpeed = 0.75f;
	UPROPERTY(EditAnywhere, Category = "Player")
	float AccelerationMultiplier = 2.0f;
	UPROPERTY(EditAnywhere, Category = "Player")
	float FinalMovementSpeed = 0.0f;
	
};
