// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "Tank.h"
#include "TankAIController.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

	/*
	auto ControlledTank = GetControlledTank();
	auto PlayerTank = GetPlayerTank();

	if (!ControlledTank)
	{
		UE_LOG(LogTemp, Error, TEXT("AIController not possessing a tank. Check Tank_BP Parent Class."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AI Controlled Tank: %s found Player Tank: %s"), *ControlledTank->GetName(), *PlayerTank->GetName());
	}
	*/
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick( DeltaTime );

	ATank* AIControlledTank = Cast<ATank>(GetPawn());
	
	ATank* PlayerControlledTank = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());



	if (ensure(PlayerControlledTank))
	{
		// Move Towards The Player
		MoveToActor(PlayerControlledTank, AcceptanceRadius);

		// Aim Towards The Player
		AIControlledTank->AimAt(PlayerControlledTank->GetActorLocation());

		// Fire If Ready
		AIControlledTank->Fire();
	}
	
}
/*
ATank* ATankAIController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

ATank* ATankAIController::GetPlayerTank() const
{
	AActor* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!PlayerPawn) { return nullptr; }
	return Cast<ATank>(PlayerPawn);

}
*/

