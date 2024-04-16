// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenerator.h"
#include "LevelChunk.h"
#include "VehiclePawn.h"

// Sets default values
ALevelGenerator::ALevelGenerator()
{
}

// Called when the game starts or when spawned
void ALevelGenerator::BeginPlay()
{
	Super::BeginPlay();

    UE_LOG(LogTemp, Warning, TEXT("ALevelGenerator - Object spawned"));
    if (GenerateInitialLevel())
    {
        UE_LOG(LogTemp, Warning, TEXT("ALevelGenerator - Default level loaded"));
        SetLevelState(LevelGeneratorState::DEFAULT_GENERATION_COMPLETE);
    }
}

void ALevelGenerator::GeneratePath(AVehiclePawn* mVehicleObj, LevelGeneratorState eventType)
{
    ALevelChunk* frontChunk = *mLevelStruct.Find(LevelChunkPosition::FRONT_START_POSITION);

    // its means the player in on the front and we are going to generate a new chunk
    if (frontChunk->GetIsCarOnPositionToReplicateNewPath())
    {
        FVector frontChunkLocation = frontChunk->GetActorLocation();

        // destroy actor
        ALevelChunk* backChunk = *mLevelStruct.Find(LevelChunkPosition::FRONT_END_POSITION);
        backChunk->Destroy();

        mLevelStruct.FindAndRemoveChecked(LevelChunkPosition::FRONT_END_POSITION);
        ALevelChunk* middleChunk = *mLevelStruct.Find(LevelChunkPosition::MIDDLE_POSITION);
        middleChunk->SetLevelPositionType(LevelChunkPosition::FRONT_END_POSITION);
        mLevelStruct.Add(LevelChunkPosition::FRONT_END_POSITION, middleChunk);

        mLevelStruct.FindAndRemoveChecked(LevelChunkPosition::MIDDLE_POSITION);
        frontChunk->SetLevelPositionType(LevelChunkPosition::MIDDLE_POSITION);
        mLevelStruct.Add(LevelChunkPosition::MIDDLE_POSITION, frontChunk);

        mLevelStruct.FindAndRemoveChecked(LevelChunkPosition::FRONT_START_POSITION);
        
        if (eventType == LevelGeneratorState::GENERATING_STRAIGHT_PATH)
        {
            ALevelChunk* newFrontChunk = GetWorld()->SpawnActor<ALevelChunk>(StraightChunks[0], FVector(frontChunkLocation.X += 1400, frontChunkLocation.Y, frontChunkLocation.Z), FRotator(0.0, 0.0, 0.0));
            newFrontChunk->SetLevelPositionType(LevelChunkPosition::FRONT_START_POSITION);
            mLevelStruct.Add(LevelChunkPosition::FRONT_START_POSITION, newFrontChunk);

            // set the new path for the car, always check on the front
            mVehicleObj->SetRoadSpline(newFrontChunk->GetSplineComponent());
        }
        else if (eventType == LevelGeneratorState::GENERATING_LEFT_PATH)
        {
            ALevelChunk* newFrontChunk = GetWorld()->SpawnActor<ALevelChunk>(LeftChunks[0], FVector(frontChunkLocation.X += 1400, frontChunkLocation.Y, frontChunkLocation.Z), FRotator(0.0, 0.0, 0.0));
            newFrontChunk->SetLevelPositionType(LevelChunkPosition::FRONT_START_POSITION);
            mLevelStruct.Add(LevelChunkPosition::FRONT_START_POSITION, newFrontChunk);

            // set the new path for the car, always check on the front
            mVehicleObj->SetRoadSpline(newFrontChunk->GetSplineComponent());
        }
        else // right path
        {
            ALevelChunk* newFrontChunk = GetWorld()->SpawnActor<ALevelChunk>(RightChunks[0], FVector(frontChunkLocation.X += 1400, frontChunkLocation.Y, frontChunkLocation.Z), FRotator(0.0, 0.0, 0.0));
            newFrontChunk->SetLevelPositionType(LevelChunkPosition::FRONT_START_POSITION);
            mLevelStruct.Add(LevelChunkPosition::FRONT_START_POSITION, newFrontChunk);

            // set the new path for the car, always check on the front
            mVehicleObj->SetRoadSpline(newFrontChunk->GetSplineComponent());
        }
        
    }
    else
    {
        // only at the start of the game when we spawn the car
        if (mVehicleObj && mVehicleObj->GetRoadSpline() == nullptr)
        {
            mVehicleObj->SetRoadSpline(frontChunk->GetSplineComponent());
        }
    }
}

bool ALevelGenerator::GenerateInitialLevel()
{
    // for the middle section always used the current location
    FVector3d currentPosition = GetActorLocation();

    // create a default path for when we are loading the level
    for (int8 i = static_cast<int8>(LevelChunkPosition::FRONT_START_POSITION); i <= static_cast<int8>(LevelChunkPosition::FRONT_END_POSITION); i++)
    {
        LevelChunkPosition chunkPosition = static_cast<LevelChunkPosition>(i);

        if (LevelChunkPosition::FRONT_START_POSITION == chunkPosition)
        {
            // Spawn the Blueprint instance
            ALevelChunk* frontChunk = GetWorld()->SpawnActor<ALevelChunk>(StraightChunks[0], FVector(1400, 0.0, 0.0), FRotator(0.0, 0.0, 0.0));
            frontChunk->SetLevelPositionType(LevelChunkPosition::FRONT_START_POSITION);
            mLevelStruct.Add(chunkPosition, frontChunk);
        }
        else if (LevelChunkPosition::MIDDLE_POSITION == chunkPosition)
        {
            // Spawn the Blueprint instance
            ALevelChunk* middleChunk = GetWorld()->SpawnActor<ALevelChunk>(StraightChunks[0], FVector(currentPosition.X, currentPosition.Y, currentPosition.Z), FRotator(0.0, 0.0, 0.0));
            middleChunk->SetLevelPositionType(LevelChunkPosition::MIDDLE_POSITION);
            mLevelStruct.Add(chunkPosition, middleChunk);
        }
        else
        {
            // Spawn the Blueprint instance
            ALevelChunk* backChunk = GetWorld()->SpawnActor<ALevelChunk>(StraightChunks[0], FVector(-1400, 0.0, 0.0), FRotator(0.0, 0.0, 0.0));
            backChunk->SetLevelPositionType(LevelChunkPosition::FRONT_END_POSITION);
            mLevelStruct.Add(chunkPosition, backChunk);
        }
    }

    // it means we were able to create the 3 level chunks correctly
    return mLevelStruct.Num() == 3;
}

