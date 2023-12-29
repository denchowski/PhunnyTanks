// Fill out your copyright notice in the Description page of Project Settings.


#include "PhunnyTanksPlayerController.h"
#include "GameFramework/Pawn.h"

void APhunnyTanksPlayerController::SetPlayerEnabledState(bool bPlayerEnabled)
{
	APawn* PlayerPawn = GetPawn();
	if (PlayerPawn != nullptr)
	{
		if (bPlayerEnabled)
		{
			PlayerPawn->EnableInput(this);
		}
		else
		{
			PlayerPawn->DisableInput(this);
		}
	}

	bShowMouseCursor = bPlayerEnabled;
}