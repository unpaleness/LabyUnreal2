#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/Pawn.h"

#include "LabyPawn.generated.h"

UCLASS()
class LABY_API ALabyPawn : public APawn {

	GENERATED_BODY()

public:

	ALabyPawn();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE UCameraComponent* GetCamera() const { return Camera; };

protected:

	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere, Category = "Setup")
	UMaterialInterface* PPOutlineMaterial;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UFloatingPawnMovement* Movement;

private:

	UPROPERTY()
	UMaterialInstanceDynamic* PPOutlineMaterialDynamic;
};
