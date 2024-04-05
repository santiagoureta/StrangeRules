// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelChunk.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "VehiclePawn.h"
#include "LevelInstance/LevelInstanceComponent.h"

ALevelChunk::ALevelChunk()
{
    StartRenderingBox = CreateDefaultSubobject<UBoxComponent>(TEXT("StartRenderingBox"));
    StartRenderingBox->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
    StartRenderingBox->SetBoxExtent(FVector(150.0f,300.0f,32.0f));

    StartRenderingBox->OnComponentBeginOverlap.AddDynamic(this, &ALevelChunk::OnBeginGeneratingMap);
    StartRenderingBox->OnComponentEndOverlap.AddDynamic(this, &ALevelChunk::OnEndGeneratingMap);
}

void ALevelChunk::OnBeginGeneratingMap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (ChunkPositionType == LevelChunkPosition::FRONT_POSITION)
    {
        // Check if the overlapped actor is valid
        if (OtherActor && OtherActor != this && OtherActor->IsA(AVehiclePawn::StaticClass()))
        {
            SetIsCarOnPositionToReplicateNewPath(true);
        }
    }
}

void ALevelChunk::OnEndGeneratingMap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (ChunkPositionType == LevelChunkPosition::FRONT_POSITION)
    {
        // Check if the overlapped actor is valid
        if (OtherActor && OtherActor != this && OtherActor->IsA(AVehiclePawn::StaticClass()))
        {
            SetIsCarOnPositionToReplicateNewPath(false);
        }
    }
}
