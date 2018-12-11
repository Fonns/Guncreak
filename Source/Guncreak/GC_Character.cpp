// Fill out your copyright notice in the Description page of Project Settings.

#include "GC_Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"

// Sets default values
AGC_Character::AGC_Character()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->bUsePawnControlRotation = false;
	CameraComponent->SetupAttachment(SpringArmComponent);
}

// Called when the game starts or when spawned
void AGC_Character::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AGC_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGC_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AGC_Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGC_Character::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AGC_Character::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookSide", this, &AGC_Character::AddControllerYawInput);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AGC_Character::PlayerCrouch);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
}

void AGC_Character::MoveForward(float Value) {

	AddMovementInput(GetActorForwardVector(), Value);
}

void AGC_Character::MoveRight(float Value) {

	AddMovementInput(GetActorRightVector(), Value);
}

void AGC_Character::PlayerCrouch() {

	if (GetMovementComponent()->IsCrouching())
	{
		UnCrouch();
	}
	else
	{
		Crouch();
		GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	}
}
