// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EventActor.generated.h"

UENUM(BlueprintType)
enum class EventType : uint8
{
    INVALID_STATE UMETA(DisplayName = "INVALID_STATE"),
    MAP_GENERATION_STRAIGHT UMETA(DisplayName = "MAP_GENERATION_STRAIGHT"),
    MAP_GENERATION_LEFT UMETA(DisplayName = "MAP_GENERATION_LEFT"),
    MAP_GENERATION_RIGHT UMETA(DisplayName = "MAP_GENERATION_RIGHT"),
    TRUCK_LIGHTS_FLICKERING UMETA(DisplayName = "TRUCK_LIGHTS_FLICKERING")
};

USTRUCT(BlueprintType)
struct FEventObject : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    uint8 EventId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EventType EvenTypeId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    uint8 NextEvenId;
};

UCLASS()
class STRANGERULES_API AEventActor : public AActor
{
    GENERATED_BODY()

public:

    // Sets default values for this actor's properties
    AEventActor();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    //! gets the current event type 
    EventType GetEventType() { return mCurrentEventType; };

protected:

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

private:

    //! set the current event type we are executing
    void SetEventType(EventType newValue) { mCurrentEventType = newValue; };

    //! the current event type
    EventType mCurrentEventType = EventType::INVALID_STATE;
};
