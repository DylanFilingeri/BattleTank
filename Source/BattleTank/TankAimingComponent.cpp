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
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTankAimingComponent::BeginPlay()
{
	// so that tank's can't fire immediately on play, need to reload
	LastFireTime = FPlatformTime::Seconds();
}

void UTankAimingComponent::Initialize(UTankBarrel * BarrelToSet, UTankTurret * TurretToSet)
{
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}

void UTankAimingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	if (Ammo <= 0)
	{
		FiringState = EFiringState::NoAmmo;
	}
	else if ((FPlatformTime::Seconds() - LastFireTime) < ReloadTimeInSeconds) // can use GetWorld()->GetTimeSeconds();
	{
		FiringState = EFiringState::Reloading;
	}
	
	else if (IsBarrelMoving())
	{
		FiringState = EFiringState::Aiming;
	}
	else
	{
		FiringState = EFiringState::Locked;
	}
}

EFiringState UTankAimingComponent::GetFiringState() const
{
	return FiringState;
}

int32 UTankAimingComponent::GetAmmoLeft() const
{
	return Ammo;
}

void UTankAimingComponent::AimAt(FVector HitLocation)
{
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
		AimDirection = OutLaunchVelocity.GetSafeNormal(); // get a vector of the same direction, but of unit length (which is equal to 1)
		MoveBarrelTowards(AimDirection);
	}
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
	if (!ensure(Barrel && Turret)) { return; }
	// work out difference between current barrel rotation, and AimDirection
	FRotator BarrelRotator = Barrel->GetForwardVector().Rotation(); // can use GetComponentRotation() as well
	FRotator AimAsRotator = AimDirection.ToOrientationRotator(); // or use AimDirection.Rotation(); [identical, but preserved for legacy]
	FRotator DeltaRotator = AimAsRotator - BarrelRotator; // difference in rotation

	// Always Yaw the shortest way
	Barrel->Elevate(DeltaRotator.Pitch);
	if (FMath::Abs(DeltaRotator.Yaw) < 180)
	{
		Turret->Rotate(DeltaRotator.Yaw);
	}
	else // avoid going the long way around
	{
		Turret->Rotate(-DeltaRotator.Yaw);
	}

}

void UTankAimingComponent::Fire()
{

	if (FiringState == EFiringState::Locked || FiringState == EFiringState::Aiming)
	{
		if (!ensure(Barrel && ProjectileBlueprint)) { return; }

		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>
		(
			ProjectileBlueprint, Barrel->GetSocketLocation(FName("Projectile")), Barrel->GetSocketRotation(FName("Projectile"))
		);

		Projectile->LaunchProjectile(LaunchSpeed);

		Ammo = Ammo - 1;
		LastFireTime = FPlatformTime::Seconds();
	}
}

bool UTankAimingComponent::IsBarrelMoving()
{
	if (!ensure(Barrel)) { return false; }

	FVector BarrelForward = Barrel->GetForwardVector();

	return !(BarrelForward.Equals(AimDirection, 0.01f)); // check if vectors are equal

}
