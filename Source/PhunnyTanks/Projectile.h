// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class PHUNNYTANKS_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(EditAnywhere, Category = "Combat")
	float Damage = 50.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float VoxelDamage = 75.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float InitialSpeed = 1500.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float MaxSpeed = 1500.f;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UStaticMeshComponent* ProjectileMesh;
	
	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* MovementComponent;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere, Category = "Combat")
	class UParticleSystem* HitParticles;

	UPROPERTY(EditAnywhere, Category = "Combat")
	class UParticleSystemComponent* TrailParticles;

	UPROPERTY(EditAnywhere, Category = "Combat")
	class USoundBase* LaunchSound;
	UPROPERTY(EditAnywhere, Category = "Combat")
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<class UCameraShakeBase> HitCameraShakeClass;
};
