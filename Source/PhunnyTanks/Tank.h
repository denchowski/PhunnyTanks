// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "InputActionValue.h"
#include "Tank.generated.h"

UCLASS()
class PHUNNYTANKS_API ATank : public ABasePawn
{
	GENERATED_BODY()
	
public:
	ATank();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaTime) override;

	void HandleDestruction();

	APlayerController* GetTankPlayerController() const { return TankPlayerController; }

	bool bAlive = true;

protected:
	virtual void BeginPlay() override;

private:
	float SuspensionMinTravel;
	float SuspensionMaxTravel;

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float Speed = 750.0f;

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float TurnRate = 250.0f;

	/* This will make suspension length equal SuspensionDistance * SuspensionPoint */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true", ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float SuspensionPoint = 0.75;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float SuspensionDistance = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float SuspensionStrength = 150;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float SuspensionDamping = 50;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = "Components")
	class UParticleSystem* MoveParticles;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* InputMapping;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* InputMoveForward;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* InputTurn;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* InputRotateTurret;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* InputHeight;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* InputFire;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* InputCamera;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* InputResetRotation;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* InputPitchRotation;

	void Move(const FInputActionValue& Value);
	void MoveEveryWheel(UStaticMeshComponent* WheelComp, float Force);
	void Turn(const FInputActionValue& Value);
	void Rotate(const FInputActionValue& Value);
	void Fire();
	void CameraZoom(const FInputActionValue& Value);
	void HandleSuspension(float DeltaTime, UStaticMeshComponent* WheelComp);
	void ChangeHeight(const FInputActionValue& Value);
	void ChangePitchRotation(const FInputActionValue& Value);

	APlayerController* TankPlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* FrontLeftWheel;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* FrontRightWheel;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* RearLeftWheel;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* RearRightWheel;

	TArray<UStaticMeshComponent*> Wheels;

	// debug functions
	void ResetRotation();
	void PrintCurrentDistance(float Distance, FVector LineStart);
	void DrawForce(FVector LineStart, FVector LineEnd);
	void DrawParticles(UParticleSystem* Particles, FVector Position, FRotator Rotation);
};
