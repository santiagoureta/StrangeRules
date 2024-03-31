// Fill out your copyright notice in the Description page of Project Settings.


#include "EventActor.h"
#include "Engine/World.h"
#include "PlayerCharacter.h"
#include "Containers/Map.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEventActor::AEventActor()
{
    PrimaryActorTick.bCanEverTick = true;
}

// Called when we change from the main menu into the map
bool AEventActor::GenerateInitialLevel()
{
    bool isSuccess = false;
    if (BlueprintClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("Level generation is starting"));
        SetEventLevelState(EventLevelState::LOADING_LEVEL);

        // for the middle section always used the current location
        FVector3d currentPosition = GetActorLocation();

        // create a default path for when we are loading the level
        for (int8 i = static_cast<int8>(LevelChunkPosition::FRONT_START_POSITION); i <= static_cast<int8>(LevelChunkPosition::FRONT_END_POSITION); i++)
        {
            LevelChunkPosition chunkPosition = static_cast<LevelChunkPosition>(i);

            if (LevelChunkPosition::FRONT_POSITION == chunkPosition)
            {
                // Spawn the Blueprint instance
                ALevelChunk* frontChunk = GetWorld()->SpawnActor<ALevelChunk>(BlueprintClass, FVector(1000, 0.0, 0.0), FRotator(0.0, 0.0, 0.0));
                mLevelStructMap.Add(chunkPosition, frontChunk);
            }
            else if (LevelChunkPosition::MIDDLE_POSITION == chunkPosition)
            {
                // Spawn the Blueprint instance
                ALevelChunk* middleChunk = GetWorld()->SpawnActor<ALevelChunk>(BlueprintClass, FVector(currentPosition.X, currentPosition.Y, currentPosition.Z), FRotator(0.0, 0.0, 0.0));
                mLevelStructMap.Add(chunkPosition, middleChunk);
            }
            else
            {
                // Spawn the Blueprint instance
                ALevelChunk* backChunk = GetWorld()->SpawnActor<ALevelChunk>(BlueprintClass, FVector(-1000, 0.0, 0.0), FRotator(0.0, 0.0, 0.0));
                mLevelStructMap.Add(chunkPosition, backChunk);
            }
        }

        isSuccess = !mLevelStructMap.IsEmpty();

        // if it goes well we just change the state
        if (isSuccess)
        {
            SetEventLevelState(EventLevelState::LEVEL_READY);
            UE_LOG(LogTemp, Warning, TEXT("Level loaded succesfully"));
        }
    }
    else
    {
        // this mean that we need to exit the game
        SetEventLevelState(EventLevelState::INVALID_STATE);
        UE_LOG(LogTemp, Warning, TEXT("Level generation failed"));
    }
    
    return isSuccess;
}

// Called each tick to know if we need to load a new level infront and delete the back one
void AEventActor::GenerateDrivingLevel()
{
    ALevelChunk* frontChunk = *mLevelStructMap.Find(LevelChunkPosition::FRONT_POSITION);
    // its means the player in on the front and we are going to generate a new chunk
    if (frontChunk->GetIsPlayerOn())
    {
        FVector frontChunkLocation = frontChunk->GetActorLocation();

        // Spawn the Blueprint instance
        ALevelChunk* newFrontChunk = GetWorld()->SpawnActor<ALevelChunk>(BlueprintClass, FVector(frontChunkLocation.X += 1000, frontChunkLocation.Y, frontChunkLocation.Z), FRotator(0.0, 0.0, 0.0));
        ALevelChunk* middleChunk = *mLevelStructMap.Find(LevelChunkPosition::MIDDLE_POSITION);

        // destroy actor
        ALevelChunk* backChunk = *mLevelStructMap.Find(LevelChunkPosition::BACK_POSITION);
        backChunk->Destroy();
        mLevelStructMap.FindAndRemoveChecked(LevelChunkPosition::BACK_POSITION);
        mLevelStructMap.Add(LevelChunkPosition::BACK_POSITION, middleChunk);
        mLevelStructMap.FindAndRemoveChecked(LevelChunkPosition::MIDDLE_POSITION);
        mLevelStructMap.Add(LevelChunkPosition::MIDDLE_POSITION, frontChunk);
        mLevelStructMap.FindAndRemoveChecked(LevelChunkPosition::FRONT_POSITION);
        mLevelStructMap.Add(LevelChunkPosition::FRONT_POSITION, newFrontChunk);
    }
}

// Called when the game starts or when spawned
void AEventActor::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void AEventActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (mCurrentLevelState == EventLevelState::DRIVING_STATE)
    {
        UE_LOG(LogTemp, Warning, TEXT("The game is ready to drive and generate path"));

        GenerateDrivingLevel();
    }
}

