// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class STRANGERULES_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

    //! Camera component
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "APlayerCharacter::CameraComponent", meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* FollowCamera;

    //! Function to modify the player stamina
    void IsPlayerRunning();

    //! Function that creates the forward-backwards movement for the player
    void MoveForward(float Value);

    //! Function that creates the right-left movement for the player
    void MoveRight(float Value);

    //! Function that Start the running action
    void StartRunning();

    //! Function that Stop the running action
    void StopRunning();

    //! Flag to know if the player is running
    UPROPERTY(EditAnywhere, Category = "APlayerCharacter::PlayerState", meta = (AllowPrivateAccess = "true"))
    bool PlayerIsRunning;

    //! Player Max walk speed
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "APlayerCharacter::DebugPlayerProperties", meta = (AllowPrivateAccess = "true"))
    float PlayerMaxWalkSpeed = 250.f;

    //! Player Max run speed
    UPROPERTY(EditAnywhere, Category = "APlayerCharacter::DebugPlayerProperties", meta = (AllowPrivateAccess = "true"))
    float PlayerMaxRunSpeed = 850.f;

    //! Player Current Stamina
    UPROPERTY(EditAnywhere, Category = "APlayerCharacter::DebugPlayerProperties", meta = (AllowPrivateAccess = "true"))
    float CurrentPlayerStamina = 0.0f;

    //! Player Max Stamina
    UPROPERTY(VisibleAnywhere, Category = "APlayerCharacter::DebugPlayerProperties", meta = (AllowPrivateAccess = "true"))
    float PlayerMaxStamina = 1000.0f;

    //! Player Min Stamina
    const float PlayerMinStamina = -150.0f;

    //! Player Min Stamina
    float PlayerDelayStamina = 0.0f;
};
