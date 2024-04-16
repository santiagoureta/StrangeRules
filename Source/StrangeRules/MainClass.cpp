// Fill out your copyright notice in the Description page of Project Settings.


#include "MainClass.h"
#include "LevelGenerator.h"
#include "VehiclePawn.h"
#include "EventActor.h"

// Sets default values
AMainClass::AMainClass()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMainClass::BeginPlay()
{
	Super::BeginPlay();
	
    if (LevelGeneratorClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("AMainClass - Level generation is starting"));

        // start loading the initial level
        SetGameState(MainGameState::LOADING_LEVEL);

        // for the middle section always used the current location
        FVector3d currentPosition = GetActorLocation();

        // Spawn the Blueprint instance
        mLevelGenerator = GetWorld()->SpawnActor<ALevelGenerator>(LevelGeneratorClass, FVector(currentPosition.X, currentPosition.Y, currentPosition.Z), FRotator(0.0, 0.0, 0.0));
        if (mLevelGenerator)
        {
            // the initial level is loaded succesfully
            const bool success = mLevelGenerator->GetLevelState() == LevelGeneratorState::DEFAULT_GENERATION_COMPLETE;
            if (success)
            {
                UE_LOG(LogTemp, Warning, TEXT("AMainClass - Level generation is complete"));

                if (TruckClass)
                {
                    // Spawn the Blueprint instance
                    mTruckObj = GetWorld()->SpawnActor<AVehiclePawn>(TruckClass, FVector(currentPosition.X += 600, currentPosition.Y += 600, currentPosition.Z), FRotator(0.0, 0.0, 0.0));
                    if (mTruckObj)
                    {
                        UE_LOG(LogTemp, Warning, TEXT("AMainClass - Truck spawned succesfully"));

                        if (EventClass)
                        {
                            mEventActor = GetWorld()->SpawnActor<AEventActor>(EventClass, FVector(currentPosition.X, currentPosition.Y, currentPosition.Z), FRotator(0.0, 0.0, 0.0));

                            if (mEventActor)
                            {
                                UE_LOG(LogTemp, Warning, TEXT("AMainClass - Event actor has been succesfully created"));
                                SetGameState(MainGameState::LEVEL_READY);
                            }
                            else
                            {
                                UE_LOG(LogTemp, Warning, TEXT("AMainClass - There was a problem while trying to spawn the event actor"));
                            }
                        }
                        else
                        {
                            UE_LOG(LogTemp, Warning, TEXT("AMainClass - There is none event class reference on the main class bp"));
                        }
                    }
                    else
                    {
                        UE_LOG(LogTemp, Warning, TEXT("AMainClass - The truck was not able to spawn correcly on the world"));
                        SetGameState(MainGameState::INVALID_STATE);
                    }
                }
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("AMainClass - The level generation failed"));
                SetGameState(MainGameState::INVALID_STATE);
            }
        }
    }
    else
    {
        // probably needs to exit the game
        UE_LOG(LogTemp, Warning, TEXT("AMainClass - we were not able to find the class for the truck, we are not able to continue creating the level"));
    }
}

// Called every frame
void AMainClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
    // it means it loaded correctly the level
    if (mGameState == MainGameState::LEVEL_READY)
    {
        // we only start driving when the flag is false
        if (!mTruckObj->GetIsDriving())
        {
            // start driving
            mTruckObj->StartDriving();

            SetGameState(MainGameState::SIMULATING_PATH_STATE);
        }
    }
    else if (mGameState == MainGameState::SIMULATING_PATH_STATE)
    {
        EventType currentEventType = mEventActor->GetEventType();
        LevelGeneratorState newState;
        switch (currentEventType)
        {
            case EventType::MAP_GENERATION_STRAIGHT:
                newState = LevelGeneratorState::GENERATING_STRAIGHT_PATH;
                break;

            case EventType::MAP_GENERATION_LEFT:
                newState = LevelGeneratorState::GENERATING_STRAIGHT_PATH;
                break;

            case EventType::MAP_GENERATION_RIGHT:
                newState = LevelGeneratorState::GENERATING_RIGHT_PATH;
                break;
        }

        // probably needs to exit the game
        UE_LOG(LogTemp, Warning, TEXT("AMainClass - new generating state %d"), static_cast<int>(newState));

        // we start generating the map, only when the user reaches the front chunk box
        mLevelGenerator->GeneratePath(mTruckObj, newState);
    }
}

