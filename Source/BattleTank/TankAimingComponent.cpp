// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"
#include "TankAimingComponent.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UTankAimingComponent::Initialize(UTankBarrel * BarrelToSet, UTankTurret * TurretToSet)
{
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}

/*
void UTankAimingComponent::SetBarrelReference(UTankBarrel* BarrelToSet)
{
	if (!ensure(BarrelToSet)) { return; }
	Barrel = BarrelToSet;
}

void UTankAimingComponent::SetTurretReference(UTankTurret* TurretToSet)
{
	if (!ensure(TurretToSet)) { return; }
	Turret = TurretToSet;
}
*/

void UTankAimingComponent::AimAt(FVector HitLocation)
{
	//FString TankName = GetOwner()->GetName(); 
	//FVector BarrelLocation = Barrel->GetComponentLocation();
	//UE_LOG(LogTemp, Warning, TEXT("%s is aiming at %s from %s"), *TankName, *HitLocation.ToString(), *BarrelLocation.ToString());

	if (!ensure(Barrel)) { return; }
	FVector OutLaunchVelocity; // out parameter
	FVector StartLocation = Barrel->GetSocketLocation(FName ("Projectile"));
	bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity
	(
		this, // (WorldContextObject) reference to the TankAimingComponent
		OutLaunchVelocity,
		StartLocation,
		HitLocation,
		LaunchSpeed,
		false, // don't choose high arc // don't need to include these values as they are defaults
		0, // no collision radius, precise
		0, // don't overide gravity
		ESuggestProjVelocityTraceOption::DoNotTrace // don't debug trace
	);

	if (bHaveAimSolution)
	{
		// calculate the OutLaunchVelocity 
		FVector AimDirection = OutLaunchVelocity.GetSafeNormal(); // get a vector of the same direction, but of unit length (which is equal to 1)
		MoveBarrelTowards(AimDirection);
		MoveTurretTowards(AimDirection);
	}
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
	if (!ensure(Barrel)) { return; }
	// work out difference between current barrel rotation, and AimDirection
		//FRotator BarrelRotator = Barrel->GetComponentRotation();
	FRotator BarrelRotator = Barrel->GetForwardVector().Rotation(); // can use GetComponentRotation() as well
	FRotator AimAsRotator = AimDirection.ToOrientationRotator(); // or use AimDirection.Rotation(); [identical, but preserved for legacy]
	FRotator DeltaRotator = AimAsRotator - BarrelRotator; // difference in rotation
	//UE_LOG(LogTemp, Warning, TEXT("BarrelRotation: %s"), *AimAsRotator.ToString());

	Barrel->Elevate(DeltaRotator.Pitch);
}

void UTankAimingComponent::MoveTurretTowards(FVector AimDirection)
{
	if (!ensure(Turret)) { return; }
	FRotator TurretRotator = Turret->GetComponentRotation();
	FRotator AimAsRotator = AimDirection.ToOrientationRotator();
	FRotator DeltaRotator = AimAsRotator - TurretRotator;

	Turret->Rotate(DeltaRotator.Yaw);
}


void UTankAimingComponent::Fire()
{
	if (!ensure(Barrel && ProjectileBlueprint)) { return; }
	bool isReloaded = (FPlatformTime::Seconds() - LastFireTime) > ReloadTimeInSeconds;

	if (isReloaded)
	{
		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>
			(
				ProjectileBlueprint, Barrel->GetSocketLocation(FName("Projectile")), Barrel->GetSocketRotation(FName("Projectile"))
			);

		Projectile->LaunchProjectile(LaunchSpeed);

		LastFireTime = FPlatformTime::Seconds();
	}
}