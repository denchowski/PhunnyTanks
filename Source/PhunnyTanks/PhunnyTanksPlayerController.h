// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PhunnyTanksPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PHUNNYTANKS_API APhunnyTanksPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	void SetPlayerEnabledState(bool bPlayerEnabled);
};
