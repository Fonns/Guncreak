// Fill out your copyright notice in the Description page of Project Settings.

#include "GC_Character.h"
#include "GC_WeaponBase.h"
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

	PlayerIsAiming = false;
	DefaultAimFOV = CameraComponent->FieldOfView;
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
}

// Called every frame
void AGC_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	float FinalFOV = PlayerIsAiming ? AimingFOV : DefaultAimFOV;
	CameraComponent->SetFieldOfView(FMath::FInterpTo(CameraComponent->FieldOfView, FinalFOV, DeltaTime, AimingInterpSpeed));
}

// Called to bind functionality to input
void AGC_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AGC_Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGC_Character::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AGC_Character::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookSide", this, &AGC_Character::AddControllerYawInput);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AGC_Character::Fire);
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &AGC_Character::AimWeapon);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &AGC_Character::AimWeaponCancel);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AGC_Character::PlayerCrouch);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AGC_Character::PickUpWeapon);
}

FVector AGC_Character::GetPawnViewLocation() const {

	if (CameraComponent)
	{
		return CameraComponent->GetComponentLocation();
	}

	return Super::GetPawnViewLocation();
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
	}
}

void AGC_Character::AimWeapon() {

	if (CurrentWeapon)
	{
		PlayerIsAiming = true;
	}
}

void AGC_Character::AimWeaponCancel() {

	PlayerIsAiming = false;
}

void AGC_Character::Fire() {

	if (CurrentWeapon)
	{
		CurrentWeapon->Fire();
	}
}

void AGC_Character::PickUpWeapon() {

	FActorSpawnParameters WeaponSpawnParams;
	WeaponSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	CurrentWeapon = GetWorld()->SpawnActor<AGC_WeaponBase>(RifleBP, FVector::ZeroVector, FRotator::ZeroRotator, WeaponSpawnParams);
	if (CurrentWeapon)
	{
		CurrentWeapon->SetOwner(this);
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "Rifle_Attach");
	}
}