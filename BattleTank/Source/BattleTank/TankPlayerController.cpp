// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "Engine/World.h"
#include "Tank.h"
#include "TankAimingComponent.h"

ATankPlayerController::ATankPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	auto AimingComponent = GetControlledTank()->FindComponentByClass<UTankAimingComponent>();

	if (AimingComponent)
	{
		this->AimingComponent = AimingComponent;
		FoundAimingComponent(AimingComponent);
	}
}

void ATankPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	AimTowardsCrosshair();
}

ATank* ATankPlayerController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

void ATankPlayerController::AimTowardsCrosshair()
{
	if (!ensure(GetControlledTank())) { return; }

	FVector HitLocation;

	if (GetSightRayHitLocation(HitLocation))
	{
		this->AimingComponent->AimAt(HitLocation);
	}

}

bool ATankPlayerController::GetSightRayHitLocation(FVector& OutHitLocation) const
{
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);

	auto ScreenLocation = FVector2D(ViewportSizeX * CrosshairXLocation, ViewportSizeY * CrosshairYLocation);

	FVector LookDirection;
	if (GetLookDirection(ScreenLocation, LookDirection))
	{

		return GetLookVectorHitLocation(LookDirection, OutHitLocation);

	}

	return false;
}

bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& OutLookDirection) const
{
	FVector CameraWorldLocation;
	return DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, CameraWorldLocation, OutLookDirection);
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector & OutHitLocation) const
{
	FHitResult HitResult;
	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	auto EndLocation = StartLocation + LookDirection * LineTraceRange;
	if (GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECC_Visibility
	))
	{
		OutHitLocation = HitResult.Location;
		return true;
	}

	OutHitLocation = FVector(0);
	return false;
}
