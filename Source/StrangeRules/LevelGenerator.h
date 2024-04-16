// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelGenerator.generated.h"

//! Event of the current phase we are in, to determine each aspect of the level spawner and the next steps of the flow
UENUM(BlueprintType)
enum class LevelGeneratorState : uint8
{
    INVALID_STATE UMETA(DisplayName = "INVALID_STATE"),
    DEFAULT_GENERATION_COMPLETE UMETA(DisplayName = "DEFAULT_GENERATION_COMPLETE"),
    GENERATING_STRAIGHT_PATH UMETA(DisplayName = "GENERATING_STRAIGHT_PATH"),
    GENERATING_LEFT_PATH UMETA(DisplayName = "GENERATING_LEFT_PATH"),
    GENERATING_RIGHT_PATH UMETA(DisplayName = "GENERATING_RIGHT_PATH")
};

UENUM(BlueprintType)
enum class LevelChunkPosition : uint8
{
    INVALID_POSITION_TYPE UMETA(DisplayName = "INVALID_POSITION_TYPE"),
    FRONT_START_POSITION  UMETA(DisplayName = "FRONT_POSITION"),
    MIDDLE_POSITION UMETA(DisplayName = "MIDDLE_POSITION"),
    FRONT_END_POSITION UMETA(DisplayName = "BACK_POSITION")
};

UCLASS()
class STRANGERULES_API ALevelGenerator : public AActor
{
	GENERATED_BODY()
	
public:

	//! Sets default values for this actor's properties
	ALevelGenerator();

    //! Gets the level generator state
    LevelGeneratorState GetLevelState() { return mLevelState; };

    //! generates the path for the game
    void GeneratePath(class AVehiclePawn* mVehicleObj, LevelGeneratorState eventType);

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

    //! Set Level State
    void SetLevelState(LevelGeneratorState newState) { mLevelState = newState; };

    //! Set Next Level State
    void SetNextLevelState(LevelGeneratorState newState) { mNextLevelState = newState; };

    //! Set Previous Level State
    void SetPreviousLevelState(LevelGeneratorState newState) { mPreviousLevelState = newState; };

    //! Called when the game starts to create the stream level
    bool GenerateInitialLevel();
    
    //! level current generation state
    LevelGeneratorState mLevelState = LevelGeneratorState::INVALID_STATE;

    //! level current generation state
    LevelGeneratorState mNextLevelState = LevelGeneratorState::INVALID_STATE;

    //! level current generation state
    LevelGeneratorState mPreviousLevelState = LevelGeneratorState::INVALID_STATE;

    // level structure
    TMap<LevelChunkPosition, class ALevelChunk*> mLevelStruct;

    //! List of straight chunks
    UPROPERTY(EditAnywhere, Category = "Setup")
    TArray<TSubclassOf<class ALevelChunk>> StraightChunks;

    //! List of Left chunks
    UPROPERTY(EditAnywhere, Category = "Setup")
    TArray<TSubclassOf<class ALevelChunk>> LeftChunks;

    //! List of Right chunks
    UPROPERTY(EditAnywhere, Category = "Setup")
    TArray<TSubclassOf<class ALevelChunk>> RightChunks;
};
