// Fill out your copyright notice in the Description page of Project Settings.


#include "VehiclePawn.h"
#include "ChaosVehicleMovementComponent.h"

AVehiclePawn::AVehiclePawn()
{
    mChaosMovementComp = GetVehicleMovementComponent();
}

void AVehiclePawn::StartDriving()
{
    if (mChaosMovementComp)
    {
        mChaosMovementComp->SetThrottleInput(mTrottleValue);
        SetIsDriving(true);

        UE_LOG(LogTemp, Warning, TEXT("AVehiclePawn - we start driving the vehicle"));
    }
}
