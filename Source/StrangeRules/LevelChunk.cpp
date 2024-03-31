// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelChunk.h"
#include "Components/BoxComponent.h"
#include "LevelInstance/LevelInstanceComponent.h"

ALevelChunk::ALevelChunk()
{
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("StartRenderingBox"));
    CollisionBox->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
    CollisionBox->SetBoxExtent(FVector(500.0f,300.0f,32.0f));

    CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ALevelChunk::OnBeginOverlap);
    CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ALevelChunk::OnEndOverlap);
}

void ALevelChunk::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (ChunkPositionType == LevelChunkPosition::FRONT_POSITION)
    {
        // Check if the overlapped actor is valid
        if (OtherActor && OtherActor != this && OtherActor->IsA(APlayerCharacter::StaticClass()))
        {
            SetIsPlayerOn(true);
        }
    }
}

void ALevelChunk::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (ChunkPositionType == LevelChunkPosition::FRONT_POSITION)
    {
        // Check if the overlapped actor is valid
        if (OtherActor && OtherActor != this && OtherActor->IsA(APlayerCharacter::StaticClass()))
        {
            SetIsPlayerOn(false);
        }
    }
}