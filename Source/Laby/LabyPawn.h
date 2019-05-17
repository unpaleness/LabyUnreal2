// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "LabyPawn.generated.h"

UCLASS()
class LABY_API ALabyPawn : public APawn {
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ALabyPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UFloatingPawnMovement* Movement;

};
