// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelInstance/LevelInstanceActor.h"
#include "LevelGenerator.h"
#include "LevelChunk.generated.h"

UENUM(BlueprintType)
enum class LevelChunkType : uint8
{
    INVALID_CHUNK_TYPE UMETA(DisplayName = "INVALID_CHUNK_TYPE"),
    SPAWN_ROAD_STRAIT UMETA(DisplayName = "ROAD_STRAIGHT"),
    SPAWN_ROAD_LEFT UMETA(DisplayName = "ROAD_LEFT"),
    SPAWN_ROAD_RIGHT UMETA(DisplayName = "ROAD_RIGHT")
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

        void SetLevelChunkType(LevelChunkType newVal) { ChunkType = newVal; };

        void SetLevelPositionType(LevelChunkPosition newVal) { ChunkPositionType = newVal; };

        bool GetIsCarOnPositionToReplicateNewPath() { return IsCarOnPositionToReplicateNewPath; };

        class USplineComponent* GetSplineComponent() { return splineComp; };

    protected:

        UPROPERTY(EditAnywhere, Category = "Setup")
        class UBoxComponent* StartRenderingBox;

        UPROPERTY(EditAnywhere, Category = "Setup")
        class USplineComponent* splineComp;

        UPROPERTY(EditAnywhere, Category = "Setup")
        bool IsCarOnPositionToReplicateNewPath = false;

        UPROPERTY(EditAnywhere, Category = "Setup")
        LevelChunkType ChunkType = LevelChunkType::INVALID_CHUNK_TYPE;

        UPROPERTY(EditAnywhere, Category = "Setup")
        LevelChunkPosition ChunkPositionType = LevelChunkPosition::INVALID_POSITION_TYPE;

        UFUNCTION()
        void OnBeginGeneratingMap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

        UFUNCTION()
        void OnEndGeneratingMap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

        void SetIsCarOnPositionToReplicateNewPath(bool newValue) { IsCarOnPositionToReplicateNewPath = newValue; };
};
