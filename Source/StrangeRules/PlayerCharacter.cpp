// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Create a camera
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(RootComponent);
    FollowCamera->bUsePawnControlRotation = true;

    // Configure character movement
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.f, 0.0f);
    GetCharacterMovement()->MaxWalkSpeed = PlayerMaxWalkSpeed;

    // Configure character actions
    PlayerIsRunning = false;
    CurrentPlayerStamina = PlayerMaxStamina;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Check if the player is running or not to change the speed and stamina for the player
    IsPlayerRunning();
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Player Movement
    PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
    PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerYawInput);

    // Player Actions
    PlayerInputComponent->BindAction("Run", IE_Pressed, this, &APlayerCharacter::StartRunning);
    PlayerInputComponent->BindAction("Run", IE_Released, this, &APlayerCharacter::StopRunning);
}

//---------------------------------------------------------------------------
//	PRIVATE
//---------------------------------------------------------------------------
// Update the config for the MoveForward state
//---------------------------------------------------------------------------
void APlayerCharacter::IsPlayerRunning()
{
    // we check if the player is running
    if (PlayerIsRunning)
    {
        // Substract the stamina from the player while running
        CurrentPlayerStamina = FMath::Clamp((CurrentPlayerStamina - 1.0f), 0.0f, PlayerMaxStamina);

        // Stop Running and add delay to the charge of the stamina
        if (CurrentPlayerStamina == 0)
        {
            PlayerDelayStamina = PlayerMinStamina;
            StopRunning();
        }
    }
    else
    {
        // we need to delay the charge of the stamina
        if (PlayerDelayStamina < 0)
        {
            PlayerDelayStamina = +FMath::Clamp((PlayerDelayStamina + 1.0f), PlayerMinStamina, PlayerMaxStamina);
        }
        // If the delay stamina is 0 we can start charging the stamina of the player
        else if (PlayerDelayStamina >= 0)
        {
            CurrentPlayerStamina = +FMath::Clamp((CurrentPlayerStamina + 1.0f), PlayerMinStamina, PlayerMaxStamina);
        }
    }
}

//---------------------------------------------------------------------------
// Manage the movement from the player (FORWARD - BACKWARDS)
//---------------------------------------------------------------------------
void APlayerCharacter::MoveForward(float Value)
{
    if ((Controller != NULL) && Value != 0.f)
    {
        // Find out which way is forward
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // Get forward vector
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

        // Add movement in that direction
        AddMovementInput(Direction, Value);
    }
}

//---------------------------------------------------------------------------
// Manage the movement from the player (RIGHT - LEFT)
//---------------------------------------------------------------------------
void APlayerCharacter::MoveRight(float Value)
{
    if ((Controller != NULL) && Value != 0.f)
    {
        // find out which way is right
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get right vector 
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        // add movement in that direction
        AddMovementInput(Direction, Value);
    }
}

//---------------------------------------------------------------------------
// Manage the movement from the player (START RUNNING)
//---------------------------------------------------------------------------
void APlayerCharacter::StartRunning()
{
    // Only start running when the stamina is greater than 0
    if (CurrentPlayerStamina >= 0)
    {
        PlayerIsRunning = true;
        GetCharacterMovement()->MaxWalkSpeed = PlayerMaxRunSpeed;
    }
}

//---------------------------------------------------------------------------
// Manage the movement from the player (STOP RUNNING)
//---------------------------------------------------------------------------
void APlayerCharacter::StopRunning()
{
    PlayerIsRunning = false;
    GetCharacterMovement()->MaxWalkSpeed = PlayerMaxWalkSpeed;
}

