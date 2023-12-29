// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Engine/World.h"
#include "Engine.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputTriggers.h"
#include "Kismet/GameplayStatics.h"


ATank::ATank()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(TurretMesh);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	FrontLeftWheel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FLW"));
	FrontLeftWheel->SetupAttachment(BaseMesh);
	Wheels.Add(FrontLeftWheel);
	FrontRightWheel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FRW"));
	FrontRightWheel->SetupAttachment(BaseMesh);
	Wheels.Add(FrontRightWheel);
	RearLeftWheel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RLW"));
	RearLeftWheel->SetupAttachment(BaseMesh);
	Wheels.Add(RearLeftWheel);
	RearRightWheel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RRW"));
	RearRightWheel->SetupAttachment(BaseMesh);
	Wheels.Add(RearRightWheel);
}

void ATank::BeginPlay()
{
	Super::BeginPlay();

	TankPlayerController = Cast<APlayerController>(GetController());
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FrontLeftWheel != nullptr)
	{
		HandleSuspension(DeltaTime, FrontLeftWheel);
	}
	if (FrontRightWheel != nullptr)
	{
		HandleSuspension(DeltaTime, FrontRightWheel);
	}
	if (RearLeftWheel != nullptr)
	{
		HandleSuspension(DeltaTime, RearLeftWheel);
	}
	if (RearRightWheel != nullptr)
	{
		HandleSuspension(DeltaTime, RearRightWheel);
	}

	//if (!TankPlayerController)
	//	return;
	//FHitResult HitResult;
	//TankPlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);
	//RotateTurret(HitResult.ImpactPoint);
}

void ATank::HandleDestruction()
{
	Super::HandleDestruction();
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	bAlive = false;
}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Get the player controller
	TankPlayerController = Cast<APlayerController>(GetController());

	// Get the local player enhanced input subsystem
	auto EISubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(TankPlayerController->GetLocalPlayer());
	
	//Add the input mapping context
	EISubsystem->AddMappingContext(InputMapping, 0);

	// Get the EnhancedInputComponent
	UEnhancedInputComponent* PlayerEIComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	//Bind Move() to the mapping
	//BindAction for enhanced system takes Action, ETriggerEvent, object, and function
	//ETriggerEvent is an enum, where Triggered means "button is held down".
	PlayerEIComp->BindAction(InputMoveForward, ETriggerEvent::Triggered, this, &ATank::Move);
	PlayerEIComp->BindAction(InputTurn, ETriggerEvent::Triggered, this, &ATank::Turn);
	PlayerEIComp->BindAction(InputRotateTurret, ETriggerEvent::Triggered, this, &ATank::Rotate);
	PlayerEIComp->BindAction(InputFire, ETriggerEvent::Triggered, this, &ATank::Fire);
	PlayerEIComp->BindAction(InputCamera, ETriggerEvent::Triggered, this, &ATank::CameraZoom);
	PlayerEIComp->BindAction(InputHeight, ETriggerEvent::Triggered, this, &ATank::ChangeHeight);
	PlayerEIComp->BindAction(InputResetRotation, ETriggerEvent::Triggered, this, &ATank::ResetRotation);
	PlayerEIComp->BindAction(InputPitchRotation, ETriggerEvent::Triggered, this, &ATank::ChangePitchRotation);
	
}

void ATank::Move(const FInputActionValue& Value)
{
	float Force = Value.Get<float>() * Speed * UGameplayStatics::GetWorldDeltaSeconds(this);

	for (auto Wheel : Wheels)
	{
		MoveEveryWheel(Wheel, Force);
	}
}

void ATank::MoveEveryWheel(UStaticMeshComponent* WheelComp, float Force)
{
	FVector ForwardForce = WheelComp->GetForwardVector() * Force;
	//ForwardForce.Z /= BaseMesh->GetMass() / 4;
	ForwardForce.Z *= - FMath::Sign(WheelComp->GetRelativeLocation().X);
	WheelComp->AddVelocityChangeImpulseAtLocation(ForwardForce, WheelComp->GetComponentLocation(), NAME_None);
	DrawParticles(MoveParticles, - WheelComp->GetForwardVector() + WheelComp->GetComponentLocation(), WheelComp->GetComponentRotation());
}

void ATank::Turn(const FInputActionValue& Value)
{
	for (auto Wheel : Wheels)
	{
		FVector ForceToApply = BaseMesh->GetRightVector().RotateAngleAxis(
			Value.Get<float>() * TurnRate,
			BaseMesh->GetRightVector() * 50) 
			* Value.Get<float>() * BaseMesh->GetMass() / 4;
		Wheel->AddImpulseAtLocation(ForceToApply * FMath::Sign(Wheel->GetRelativeLocation().X), Wheel->GetComponentLocation());
	}
}

void ATank::Rotate(const FInputActionValue& Value)
{
	if (!TankPlayerController)
	{
		return;
	}

	// FHitResult HitResult;
	// TankPlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);
	// FVector ToTarget = HitResult.ImpactPoint
	FRotator TurretRotate = FRotator::ZeroRotator;
	TurretRotate.Yaw = Value.Get<float>();
	TurretMesh->AddLocalRotation(TurretRotate, false);
}

void ATank::Fire()
{
	Super::Fire();
}

void ATank::CameraZoom(const FInputActionValue& Value)
{
	float CurrentZoom = SpringArm->TargetArmLength;
	CurrentZoom += Value.Get<float>() * 15;
	if (CurrentZoom <= 1000 || CurrentZoom >= 250)
	{
		SpringArm->TargetArmLength = CurrentZoom;
	}
}

void ATank::HandleSuspension(float DeltaTime, UStaticMeshComponent* WheelComp)
{
	FHitResult HitResult;
	FVector LineStart = WheelComp->GetComponentLocation();
	FVector LineEnd = LineStart - (WheelComp->GetUpVector() * SuspensionDistance);
	
	bool OnGround = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		LineStart,
		LineEnd,
		ECC_Camera
	);

	if (OnGround)
	{
		float CurrentSpringLength = HitResult.Distance;
		float TargetLength = SuspensionDistance * SuspensionPoint;
		
		if (CurrentSpringLength <= SuspensionDistance && CurrentSpringLength != TargetLength )
		{
			float TargetForce = (TargetLength - CurrentSpringLength) * SuspensionStrength * DeltaTime * (BaseMesh->GetMass() / 4);
			FVector WheelSpeed = WheelComp->GetComponentVelocity() * DeltaTime;
			FVector UpVector = WheelComp->GetUpVector();
			FVector UpwardForce = (UpVector * TargetForce) - (WheelSpeed * SuspensionDamping);

			BaseMesh->AddImpulseAtLocation(UpwardForce, LineStart);
			DrawParticles(MoveParticles, - WheelComp->GetUpVector() + WheelComp->GetComponentLocation(), BaseMesh->GetRelativeRotation());
		}
	}
}

void ATank::ChangeHeight(const FInputActionValue& Value)
{
	this->SuspensionDistance += Value.Get<float>();
}

void ATank::ChangePitchRotation(const FInputActionValue& Value)
{
	BaseMesh->AddRelativeRotation({Value.Get<float>(), 0, 0});
}

void ATank::ResetRotation()
{
	FRotator ZeroPitchRoll = { 0, BaseMesh->GetComponentRotation().Yaw, 0 };
	BaseMesh->SetWorldRotation(ZeroPitchRoll);
}

void ATank::PrintCurrentDistance(float Distance, FVector LineStart)
{
	FColor drawColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f).ToFColor(true);
	float drawDuration = 0.0f;
	bool drawShadow = true;
	if (GEngine != nullptr)
	{
		DrawDebugString(
			GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull),
			LineStart, 
			*FString::Printf(TEXT("%f"), Distance),
			NULL, 
			drawColor, 
			drawDuration, 
			drawShadow
		);
	}
}

void ATank::DrawForce(FVector LineStart, FVector LineEnd)
{
	DrawDebugLine(
		GetWorld(),
		LineStart,
		LineEnd,
		FColor::Red,
		false,
		-1.f
	);
}

void ATank::DrawParticles(UParticleSystem* Particles, FVector Position, FRotator Rotation)
{
	if (Particles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			this->GetWorld(),
			Particles,
			Position,
			Rotation,
			true
		);
	}
}
