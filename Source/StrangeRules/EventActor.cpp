// Fill out your copyright notice in the Description page of Project Settings.


#include "EventActor.h"
#include "Engine/World.h"

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

        // lambda thats fills the information for the chunks for the event
        auto FillChunkInfo = [](LevelChunkType type, float x, float y, float z)
        {
            ChunkObject chunkObject;
            chunkObject.chunkType = type;
            chunkObject.position_X = x;
            chunkObject.position_Y = y;
            chunkObject.position_Z = z;
            return chunkObject;
        };

        // for the middle section always used the current location
        FVector3d currentPosition = GetActorLocation();

        // create a default path for when we are loading the level
        LevelChunkType chunkType = BlueprintClass.GetDefaultObject()->GetLevelChunkType();
        for (int8 i = static_cast<int8>(LevelChunkPosition::FRONT_START_POSITION); i <= static_cast<int8>(LevelChunkPosition::FRONT_END_POSITION); i++)
        {
            ChunkObject chunkObject;
            AActor* SpawnedActor;
            switch (i)
            {
                case static_cast<int8>(LevelChunkPosition::FRONT_POSITION):
                {
                    chunkObject = FillChunkInfo(chunkType, 1000, 0.0, 0.0);

                    // Spawn the Blueprint instance
                    FVector location = FVector(chunkObject.position_X, chunkObject.position_Y, chunkObject.position_Z);
                    SpawnedActor = GetWorld()->SpawnActor<AActor>(BlueprintClass, location, FRotator(0.0, 0.0, 0.0));
                    break;
                }

                case static_cast<int8>(LevelChunkPosition::MIDDLE_POSITION):
                {
                    chunkObject = FillChunkInfo(chunkType, currentPosition.X, currentPosition.Y, currentPosition.Z);
                    // Spawn the Blueprint instance
                    FVector location = FVector(chunkObject.position_X, chunkObject.position_Y, chunkObject.position_Z);
                    SpawnedActor = GetWorld()->SpawnActor<AActor>(BlueprintClass, location, FRotator(0.0, 0.0, 0.0));
                    break;
                }

                case static_cast<int8>(LevelChunkPosition::BACK_POSITION):
                {
                    chunkObject = FillChunkInfo(chunkType, -1000, 0.0, 0.0);
                    // Spawn the Blueprint instance
                    FVector location = FVector(chunkObject.position_X, chunkObject.position_Y, chunkObject.position_Z);
                    SpawnedActor = GetWorld()->SpawnActor<AActor>(BlueprintClass, location, FRotator(0.0, 0.0, 0.0));
                    break;
                }
            }

            if (SpawnedActor)
            {
                mLevelStructMap.Add(static_cast<LevelChunkPosition>(i), SpawnedActor);
                isSuccess = true;
            }
            else
            {
                isSuccess = false;
                break;
            }
        }

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
bool AEventActor::GenerateDrivingLevel()
{
    bool isSuccess = false;
    
    //ALevelChunk* frontChunk = static_cast<ALevelChunk*>(mLevelStructMap.Find(LevelChunkPosition::FRONT_POSITION));
    return isSuccess;
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

