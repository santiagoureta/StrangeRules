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

    void SetRoadSpline(USplineComponent* roadSpline) {mRoadSplineRef = roadSpline;};

    UFUNCTION(BlueprintCallable, Category = "RoadInformation")
    class USplineComponent* GetRoadSpline() { return mRoadSplineRef; };

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    class USplineComponent* mRoadSplineRef;
};
