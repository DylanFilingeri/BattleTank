// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "TankAimingComponent.h"
#include "Tank.h" // So we can implement OnDeath
#include "TankAIController.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ATankAIController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn); // if we don't call super this whole thing won't get called on this object
	if (InPawn)
	{
		ATank* PossessedTank = Cast<ATank>(InPawn);
		if (!ensure(PossessedTank)) { return; }

		// Subscribe our local method to the tank's death event
		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnTankDeath);
	}
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
		AimingComponent->Fire();
	}
	

}

void ATankAIController::OnTankDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("AI Controller reporting dead tank"));
}

