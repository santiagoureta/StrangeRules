// Fill out your copyright notice in the Description page of Project Settings.
#include "EventActor.h"
#include "Engine/World.h"
#include "PlayerCharacter.h"
#include "VehiclePawn.h"
#include "LevelGenerator.h"
#include "Engine/DataTable.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
AEventActor::AEventActor()
{
    PrimaryActorTick.bCanEverTick = true;
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
    
    // Generate a random integer between Min (inclusive) and Max (inclusive)
    int8 RandomNumber = FMath::RandRange(1, 3);
    switch (RandomNumber)
    {
        case 1:
            SetEventType(EventType::MAP_GENERATION_STRAIGHT);
            break;
        case 2:
            SetEventType(EventType::MAP_GENERATION_LEFT);
            break;
        case 3:
            SetEventType(EventType::MAP_GENERATION_RIGHT);
            break;
    }
}

