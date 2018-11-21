// Fill out your copyright notice in the Description page of Project Settings.

#include "TankMovementComponent.h"
#include "TankTrack.h"

void UTankMovementComponent::Initialize(UTankTrack* RightTrackToSet, UTankTrack* LeftTrackToSet)
{
	if (!RightTrackToSet || !LeftTrackToSet) { return; }

	RightTrack = RightTrackToSet;
	LeftTrack = LeftTrackToSet;
}


void UTankMovementComponent::IntentMoveForward(float Throw)
{
	RightTrack->SetThrottle(Throw);
	LeftTrack->SetThrottle(Throw);
}