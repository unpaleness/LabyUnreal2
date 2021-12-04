#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LabyPlayerController.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogLabyPlayerController, Log, All)


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
	float ForcePerSecBase = 10000.0f;

	/** Lookaround speed */
	UPROPERTY(EditAnywhere, Category = "Player")
	float LookSpeed = 0.75f;

	/** Relative speed when acceleration button is pressed */
	UPROPERTY(EditAnywhere, Category = "Player")
	float AccelerationMultiplier = 2.0f;

	/** Total movement speed including potential acceletation */
	float ForcePerSecFinal = 0.0f;

	/** Input for moving forward/backward */
	float ForwardInput = 0.0f;

	/** Input for moving right/left */
	float RightInput = 0.0f;

	/** Input for rotating horizontally */
	float YawInput = 0.0f;

	/** Input for camera rotating up/down */
	float PitchInput = 0.0f;
};
