// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Map.h"
#include "LevelChunk.h"
#include "EventActor.generated.h"

struct ChunkObject
{
    LevelChunkType chunkType;
    float position_X;
    float position_Y;
    float position_Z;
    float rotation_Yaw;
    float rotation_Pitch;
};

//! Event of the current phase we are in, to determine each aspect of the level spawner and the next steps of the flow
UENUM(BlueprintType)
enum class EventLevelState : uint8
{
    INVALID_STATE UMETA(DisplayName = "INVALID_STATE"),
    LOADING_LEVEL UMETA(DisplayName = "LOADING_LEVEL"),
    LEVEL_READY UMETA(DisplayName = "LEVEL_READY"),
    STARTING_GAME UMETA(DisplayName = "STARTING_GAME"),
    DRIVING_STATE UMETA(DisplayName = "DRIVING_STATE"),
    IDDLE_STATE UMETA(DisplayName = "IDDLE_STATE")
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

    //! Setter for the new state of the level spawner
    UFUNCTION(BlueprintCallable, Category = "LevelProperties")
    void SetEventLevelState(EventLevelState newLevelState) { mCurrentLevelState = newLevelState; };

    //! Get the current state of the level spawner
    EventLevelState GetEventLevelState() { return mCurrentLevelState; };

    //! Called when the game starts to create the stream level
    UFUNCTION(BlueprintCallable, Category = "LevelGeneration")
    bool GenerateInitialLevel();

    UFUNCTION(BlueprintCallable, Category = "LevelGeneration")
    bool GenerateDrivingLevel();

protected:

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

private:

    EventLevelState mCurrentLevelState = EventLevelState::INVALID_STATE;

    UPROPERTY(EditAnywhere, Category = "Setup")
    TSubclassOf<ALevelChunk> BlueprintClass;

    TMap<LevelChunkPosition, AActor*> mLevelStructMap;
};
