// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "VehiclePawn.generated.h"

UCLASS()
class STRANGERULES_API AVehiclePawn : public AWheeledVehiclePawn
{
    GENERATED_BODY()

public:

    AVehiclePawn();

    void SetRoadSpline(USplineComponent* roadSpline) {mRoadSplineRef = roadSpline;};

    UFUNCTION(BlueprintCallable, Category = "RoadInformation")
    class USplineComponent* GetRoadSpline() { return mRoadSplineRef; };

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    class USplineComponent* mRoadSplineRef;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float mTrottleValue = 0.5f;

    // Start the engine at the start of the game or when we need to start again
    void StartDriving();

    //! Get the flag to know if we are driving
    bool GetIsDriving() { return mIsDriving; };

private:

    //! flag to know if we are driving
    bool mIsDriving = false;

    //! vehicle movement component
    UChaosVehicleMovementComponent* mChaosMovementComp;

    //! Function that sets the flag for the vehicle
    void SetIsDriving(bool newValue) { mIsDriving = newValue; };
};
