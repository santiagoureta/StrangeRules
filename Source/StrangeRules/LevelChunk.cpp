// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelChunk.h"
#include "Components/BoxComponent.h"
#include "LevelInstance/LevelInstanceComponent.h"

//
ALevelChunk::ALevelChunk()
{
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("StartRenderingBox"));
    CollisionBox->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
    CollisionBox->SetBoxExtent(FVector(500.0f,300.0f,32.0f));
}
