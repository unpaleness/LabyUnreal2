#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Pawn.h"

#include "LabyPawn.generated.h"


DECLARE_LOG_CATEGORY_EXTERN(LogLabyPawn, Log, All)


UCLASS()
class LABY_API ALabyPawn : public APawn {

	GENERATED_BODY()

public:

	ALabyPawn();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE UStaticMeshComponent* GetMesh() const { return Mesh; }

	FORCEINLINE float GetMass() const { return GetMesh()->GetBodyInstance()->GetBodyMass(); }

	FORCEINLINE UCameraComponent* GetCamera() const { return Camera; }

protected:

	virtual void BeginPlay() override;

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* Camera;
};
