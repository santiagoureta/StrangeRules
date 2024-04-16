// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MainClass.generated.h"

//! Event of the current phase we are in, to determine each aspect of the level spawner and the next steps of the flow
UENUM(BlueprintType)
enum class MainGameState : uint8
{
    INVALID_STATE UMETA(DisplayName = "INVALID_STATE"),
    LOADING_LEVEL UMETA(DisplayName = "LOADING_LEVEL"),
    LEVEL_READY UMETA(DisplayName = "LEVEL_READY"),
    STARTING_GAME UMETA(DisplayName = "STARTING_GAME"),
    SIMULATING_PATH_STATE UMETA(DisplayName = "SIMULATING_PATH_STATE"),
    IDDLE_STATE UMETA(DisplayName = "IDDLE_STATE")
};

UCLASS()
class STRANGERULES_API AMainClass : public AActor
{
	GENERATED_BODY()
	
public:

	//! constructor
	AMainClass();

    //! Called every frame
    virtual void Tick(float DeltaTime) override;

    //! Get current game state
    MainGameState GetGameState() { return mGameState; };

protected:

	//! Called when the game starts or when spawned
	virtual void BeginPlay() override;
    
private:

    //! update the game state
    void SetGameState(MainGameState newState) { mGameState = newState; };

    //! Class ref to spawn the truck
    UPROPERTY(EditAnywhere, Category = "Setup")
    TSubclassOf<class AVehiclePawn> TruckClass;

    //! Class ref to spawn the generation of the map
    UPROPERTY(EditAnywhere, Category = "Setup")
    TSubclassOf<class ALevelGenerator> LevelGeneratorClass;

    //! Class ref to spawn the generation of the map
    UPROPERTY(EditAnywhere, Category = "Setup")
    TSubclassOf<class AEventActor> EventClass;

    //! Current game state
    MainGameState mGameState = MainGameState::INVALID_STATE;

    //! The truck obj reference
    AVehiclePawn* mTruckObj = nullptr;

    //! The level generator obj reference
    ALevelGenerator* mLevelGenerator = nullptr;

    //! manages the events for the game
    AEventActor* mEventActor = nullptr;
};
