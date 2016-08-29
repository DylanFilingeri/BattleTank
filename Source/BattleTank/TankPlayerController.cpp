// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "TankPlayerController.h"


void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ATank* ControlledTank = GetControlledTank();
	if (!ControlledTank)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController not possessing a tank. Check Tank_BP Parent Class."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Controlled Tank: %s"), *ControlledTank->GetName()/*GetFName().ToString()*/); // can use fname to string as well
	}
}

ATank* ATankPlayerController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

