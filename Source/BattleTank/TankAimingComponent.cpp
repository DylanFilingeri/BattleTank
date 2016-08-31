// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "TankBarrel.h"
#include "TankAimingComponent.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTankAimingComponent::SetBarrelReference(UTankBarrel* BarrelToSet)
{
	Barrel = BarrelToSet;
}

void UTankAimingComponent::AimAt(FVector HitLocation, float LaunchSpeed)
{
	//FString TankName = GetOwner()->GetName(); 
	//FVector BarrelLocation = Barrel->GetComponentLocation();
	//UE_LOG(LogTemp, Warning, TEXT("%s is aiming at %s from %s"), *TankName, *HitLocation.ToString(), *BarrelLocation.ToString());

	if (!Barrel) { return; }
	FVector OutLaunchVelocity; // out parameter
	FVector StartLocation = Barrel->GetSocketLocation(FName ("Projectile"));

	if (UGameplayStatics::SuggestProjectileVelocity(
		this, // (WorldContextObject) reference to the TankAimingComponent
		OutLaunchVelocity,
		StartLocation,
		HitLocation,
		LaunchSpeed,
		false, // don't choose high arc // don't need to include these values as they are defaults
		0, // no collision radius, precise
		0, // don't overide gravity
		ESuggestProjVelocityTraceOption::DoNotTrace // don't debug trace
	))
	{
		// calculate the OutLaunchVelocity 
		FVector AimDirection = OutLaunchVelocity.GetSafeNormal();
		MoveBarrelTowards(AimDirection);
		// Get Barrel Component Rotation and store as FRotator
		// Set the Barrel Rotation
		
	}
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
	// work out difference between current barrel rotation, and AimDirection
		//FRotator BarrelRotator = Barrel->GetComponentRotation();
	FRotator BarrelRotator = Barrel->GetForwardVector().Rotation(); // can use GetComponentRotation() as well
	FRotator AimAsRotator = AimDirection.ToOrientationRotator(); // or use AimDirection.Rotation(); [identical, but preserved for legacy]
	FRotator DeltaRotator = AimAsRotator - BarrelRotator; // difference in rotation
	//UE_LOG(LogTemp, Warning, TEXT("BarrelRotation: %s"), *AimAsRotator.ToString());

	Barrel->Elevate(5); // TODO magic number
}
