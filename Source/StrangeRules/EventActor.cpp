// Fill out your copyright notice in the Description page of Project Settings.


#include "EventActor.h"
#include "Engine/World.h"
#include "PlayerCharacter.h"
#include "VehiclePawn.h"
#include "Containers/Map.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
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
                ALevelChunk* frontChunk = GetWorld()->SpawnActor<ALevelChunk>(BlueprintClass, FVector(2000, 0.0, 0.0), FRotator(0.0, 0.0, 0.0));
                mLevelStructMap.Add(chunkPosition, frontChunk);
            }
            else if (LevelChunkPosition::MIDDLE_POSITION == chunkPosition)
            {
                // Spawn the Blueprint instance
                ALevelChunk* middleChunk = GetWorld()->SpawnActor<ALevelChunk>(BlueprintMiddleClass, FVector(currentPosition.X, currentPosition.Y, currentPosition.Z), FRotator(0.0, 0.0, 0.0));
                mLevelStructMap.Add(chunkPosition, middleChunk);
            }
            else
            {
                // Spawn the Blueprint instance
                ALevelChunk* backChunk = GetWorld()->SpawnActor<ALevelChunk>(BlueprintEndClass, FVector(-2000, 0.0, 0.0), FRotator(0.0, 0.0, 0.0));
                mLevelStructMap.Add(chunkPosition, backChunk);
            }
        }

        isSuccess = !mLevelStructMap.IsEmpty();

        // if it goes well we just change the state
        if (isSuccess)
        {
            UE_LOG(LogTemp, Warning, TEXT("Level loaded succesfully"));

            if (BlueprintTruckClass)
            {
                // Spawn the Blueprint instance
                AVehiclePawn* truckPawn = GetWorld()->SpawnActor<AVehiclePawn>(BlueprintTruckClass, FVector(currentPosition.X += 600, currentPosition.Y += 200, currentPosition.Z), FRotator(0.0, 0.0, 0.0));
                if (truckPawn)
                {
                    UE_LOG(LogTemp, Warning, TEXT("Truck obj created succesfully"));

                    mVehicleSpawned = truckPawn;
                    SetEventLevelState(EventLevelState::LEVEL_READY);
                }
            }
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
void AEventActor::CheckTruckPosition()
{
    ALevelChunk* frontChunk = *mLevelStructMap.Find(LevelChunkPosition::FRONT_POSITION);
    // its means the player in on the front and we are going to generate a new chunk
    if (frontChunk->GetIsCarOnPositionToReplicateNewPath())
    {
        FVector frontChunkLocation = frontChunk->GetActorLocation();

        // destroy actor
        ALevelChunk* backChunk = *mLevelStructMap.Find(LevelChunkPosition::BACK_POSITION);
        backChunk->Destroy();

        mLevelStructMap.FindAndRemoveChecked(LevelChunkPosition::BACK_POSITION);
        ALevelChunk* middleChunk = *mLevelStructMap.Find(LevelChunkPosition::MIDDLE_POSITION);
        middleChunk->SetLevelPositionType(LevelChunkPosition::BACK_POSITION);
        mLevelStructMap.Add(LevelChunkPosition::BACK_POSITION, middleChunk);

        mLevelStructMap.FindAndRemoveChecked(LevelChunkPosition::MIDDLE_POSITION);
        frontChunk->SetLevelPositionType(LevelChunkPosition::MIDDLE_POSITION);
        mLevelStructMap.Add(LevelChunkPosition::MIDDLE_POSITION, frontChunk);

        mLevelStructMap.FindAndRemoveChecked(LevelChunkPosition::FRONT_POSITION);
        ALevelChunk* newFrontChunk = GetWorld()->SpawnActor<ALevelChunk>(BlueprintClass, FVector(frontChunkLocation.X += 2000, frontChunkLocation.Y, frontChunkLocation.Z), FRotator(0.0, 0.0, 0.0));
        mLevelStructMap.Add(LevelChunkPosition::FRONT_POSITION, newFrontChunk);

        // when we finish generating and placing the new actors on the screne we need to re generate the path for the truck
        USplineComponent* newSpline = NewObject<USplineComponent>();
        newSpline->AddSplinePoint(newFrontChunk->GetActorLocation(), ESplineCoordinateSpace::World);
        newSpline->AddSplinePoint(frontChunk->GetActorLocation(), ESplineCoordinateSpace::World);
        newSpline->AddSplinePoint(middleChunk->GetActorLocation(), ESplineCoordinateSpace::World);

        if (mVehicleSpawned)
        {
            mVehicleSpawned->SetRoadSpline(newSpline);
        }
    }
    else
    {
        if (mVehicleSpawned && mVehicleSpawned->GetRoadSpline() == nullptr)
        {
            // when we finish generating and placing the new actors on the screne we need to re generate the path for the truck
            USplineComponent* newSpline = NewObject<USplineComponent>();
            newSpline->AddSplinePoint(frontChunk->GetActorLocation(), ESplineCoordinateSpace::World);
            newSpline->AddSplinePoint(mVehicleSpawned->GetActorLocation(), ESplineCoordinateSpace::World);
            mVehicleSpawned->SetRoadSpline(newSpline);
        }
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
        CheckTruckPosition();
    }
}

