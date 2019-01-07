// Fill out your copyright notice in the Description page of Project Settings.

#include "GC_Character.h"
#include "GC_WeaponBase.h"
#include "GC_HealthComponent.h"
#include "Guncreak.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"

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

	HealthComp = CreateDefaultSubobject<UGC_HealthComponent>(TEXT("HealthComponent"));

	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_BULLETS, ECR_Ignore);

	AimingFOV = 50.0f;
	AimingInterpSpeed = 30.0f;
}

// Called when the game starts or when spawned
void AGC_Character::BeginPlay()
{
	Super::BeginPlay();

	PlayerIsAiming = false;
	DefaultAimFOV = CameraComponent->FieldOfView;
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	PickUpWeapon();

	HealthComp->OnHealthChanged.AddDynamic(this, &AGC_Character::OnHealthChanged);

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

	if (Role < ROLE_Authority)
	{
		SVPickUpWeapon();
	}

	FActorSpawnParameters WeaponSpawnParams;
	WeaponSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	CurrentWeapon = GetWorld()->SpawnActor<AGC_WeaponBase>(RifleBP, FVector::ZeroVector, FRotator::ZeroRotator, WeaponSpawnParams);
	if (CurrentWeapon)
	{
		CurrentWeapon->SetOwner(this);
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "Rifle_Attach");
	}
}

void AGC_Character::OnHealthChanged(UGC_HealthComponent* HealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0.0f && !PlayerHasDied)
	{
		PlayerHasDied = true;
		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//GetMesh()->SetSimulatePhysics(true);

		DetachFromControllerPendingDestroy();
		SetLifeSpan(10.0f);
	}
}

void AGC_Character::SVPickUpWeapon_Implementation() {

	PickUpWeapon();
}

bool AGC_Character::SVPickUpWeapon_Validate() {

	return true;
}

void AGC_Character::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGC_Character, CurrentWeapon);
	DOREPLIFETIME(AGC_Character, PlayerHasDied);
}