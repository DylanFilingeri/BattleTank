// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "TankTrack.h"
#include "TankMovementComponent.h"

void UTankMovementComponent::Initialize(UTankTrack* LeftTrackToSet, UTankTrack* RightTrackToSet)
{
	// if (!LeftTrackToSet || !RightTrackToSet) { return; } // not needed

	LeftTrack = LeftTrackToSet;
	RightTrack = RightTrackToSet;
}

void UTankMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
	// No need to call Super:: as we're replacing the functionality here

	auto AIForward = GetOwner()->GetActorForwardVector().GetSafeNormal(); // GetSafeNormal makes it a unit vector
	auto AIForwardIntention = MoveVelocity.GetSafeNormal();

	// use DotProduct to find the angle between the two unit vectors
	float ForwardThrow = FVector::DotProduct(AIForward, AIForwardIntention); 
	IntendMoveForward(ForwardThrow);

	//FVector RightThrow = FVector::CrossProduct(AIForward, AIForwardIntention);
	float RightThrow = FVector::CrossProduct( AIForward, AIForwardIntention).Z; // can make this FVector a float by using .Z at the end
	IntendTurnRight(RightThrow);
}

void UTankMovementComponent::IntendMoveForward(float Throw)
{
	if (!LeftTrack || !RightTrack) { return; }

	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(Throw);
}


void UTankMovementComponent::IntendTurnRight(float Throw)
{
	if (!LeftTrack || !RightTrack) { return; }

	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(-Throw);
}