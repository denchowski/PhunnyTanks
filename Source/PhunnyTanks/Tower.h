// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tower.generated.h"

/**
 * 
 */
UCLASS()
class PHUNNYTANKS_API ATower : public ABasePawn
{
	GENERATED_BODY()
	
public:

	virtual void Tick(float DeltaTime) override;

	void HandleDestruction();

protected:

	virtual void BeginPlay() override;

private:

	bool InFireRange();

	class ATank* Target;

	UPROPERTY(EditDefaultsOnly)
	float FireRange = 1250.f;

	void CheckFireCondition();

	FTimerHandle FireRateTimerHandle;
	UPROPERTY(EditDefaultsOnly)
	float FireRate = 2.f;
};
