// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelInstance/LevelInstanceActor.h"
#include "PlayerCharacter.h"
#include "LevelChunk.generated.h"

UENUM()
enum class LevelChunkType : int8
{
    INVALID_CHUNK_TYPE = -1,
    SPAWN_ROAD_STRAIT UMETA(DisplayName = "ROAD_STRAIGHT"),
    SPAWN_ROAD_LEFT UMETA(DisplayName = "ROAD_LEFT"),
    SPAWN_ROAD_RIGHT UMETA(DisplayName = "ROAD_RIGHT")
};

UENUM()
enum class LevelChunkPosition : int8
{
    INVALID_POSITION_TYPE = -1,
    FRONT_START_POSITION  UMETA(DisplayName = "FRONT_POSITION"),
    FRONT_POSITION = FRONT_START_POSITION,
    MIDDLE_POSITION UMETA(DisplayName = "MIDDLE_POSITION"),
    BACK_POSITION UMETA(DisplayName = "BACK_POSITION"),
    FRONT_END_POSITION = BACK_POSITION
};

UCLASS()
class STRANGERULES_API ALevelChunk : public ALevelInstance
{
    GENERATED_BODY()
    
    public:

        // Sets default values for this actor's properties
        ALevelChunk();

        LevelChunkType GetLevelChunkType() { return ChunkType; };

        LevelChunkPosition GetLevelPositionType() { return ChunkPositionType; };

        bool GetIsPlayerOn() { return IsPlayerOn; };

    protected:

        UPROPERTY(EditAnywhere, Category = "Setup")
        class UBoxComponent* CollisionBox;

        UPROPERTY(EditAnywhere, Category = "Setup")
        TSubclassOf<APlayerCharacter> PlayerClass;

        UPROPERTY(EditAnywhere, Category = "Setup")
        bool IsPlayerOn = false;

        UPROPERTY(EditAnywhere, Category = "Setup")
        LevelChunkType ChunkType = LevelChunkType::INVALID_CHUNK_TYPE;

        UPROPERTY(EditAnywhere, Category = "Setup")
        LevelChunkPosition ChunkPositionType = LevelChunkPosition::INVALID_POSITION_TYPE;

        UFUNCTION()
        void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

        UFUNCTION()
        void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

        void SetIsPlayerOn(bool newValue) { IsPlayerOn = newValue; };
};
