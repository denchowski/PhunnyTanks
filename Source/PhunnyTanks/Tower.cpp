// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Target)
	{
		return;
	}

	if (InFireRange())
	{
		RotateTurret(Target->GetActorLocation());
	}
}

void ATower::HandleDestruction()
{
	Super::HandleDestruction();
	Destroy();
}

void ATower::BeginPlay() 
{
	Super::BeginPlay();

	Target = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

	GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);
}

bool ATower::InFireRange()
{
	if (Target)
	{
		float Distance = FVector::Dist(GetActorLocation(), Target->GetActorLocation());
		if (Distance <= FireRange)
		{
			return true;
		}
		Distance = FVector::Dist(TurretMesh->GetComponentLocation(), Target->GetActorLocation());
		if (Distance <= FireRange)
		{
			return true;
		}
	}
	return false;
}


void ATower::CheckFireCondition()
{
	if (Target == nullptr)
	{
		return;
	}
	if (InFireRange() && Target->bAlive)
	{
		Fire();
	}
}