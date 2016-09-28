// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "TankAimingComponent.h"
#include "TankAIController.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick( DeltaTime );
	
	APawn* AIControlledTank = GetPawn();

	APawn* PlayerControlledTank = GetWorld()->GetFirstPlayerController()->GetPawn();


	if (!ensure(PlayerControlledTank && AIControlledTank)) { return; }
	
	// Move Towards The Player
	MoveToActor(PlayerControlledTank, AcceptanceRadius);

	// Aim Towards The Player
	FVector HitLocation = PlayerControlledTank->GetActorLocation();
	UTankAimingComponent* AimingComponent = AIControlledTank->FindComponentByClass<UTankAimingComponent>();
	AimingComponent->AimAt(HitLocation);

	if (AimingComponent->GetFiringState() == EFiringState::Locked)
	{
		UE_LOG(LogTemp, Warning, TEXT("Locked"));
		AimingComponent->Fire();
	}
	

}

