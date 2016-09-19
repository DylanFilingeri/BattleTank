// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
//#include "Tank.h"
#include "TankAimingComponent.h"
#include "TankAIController.h"
// Depends on movement component via pathfinding system

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

	//ATank* AIControlledTank = Cast<ATank>(GetPawn());
	
	APawn* AIControlledTank = GetPawn();
	
	//ATank* PlayerControlledTank = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
	APawn* PlayerControlledTank = GetWorld()->GetFirstPlayerController()->GetPawn();
	//FVector HitLocation = PlayerControlledTank->GetActorLocation();


	if (!ensure(PlayerControlledTank && AIControlledTank)) { return; }
	
	// Move Towards The Player
	MoveToActor(PlayerControlledTank, AcceptanceRadius);

	// Aim Towards The Player
	//AIControlledTank->AimAt(PlayerControlledTank->GetActorLocation());
	//UTankAimingComponent* AimingComponent = AIControlledTank->FindComponentByClass<UTankAimingComponent>();
	//AIControlledTank->FindComponentByClass<UTankAimingComponent>()->AimAt(HitLocation);
	AIControlledTank->FindComponentByClass<UTankAimingComponent>()->AimAt(PlayerControlledTank->GetActorLocation());

	// TODO Fix Firing
	// Fire If Ready
	//GetPawn()->Fire();

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

