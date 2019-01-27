// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GC_Character.generated.h"

class UCameraComponent;
class USpringArmComponent;
class AGC_WeaponBase;
class UGC_HealthComponent;
class USoundCue;

UCLASS()
class GUNCREAK_API AGC_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGC_Character();

	bool IsGameOver;
	bool IsWinner;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UGC_HealthComponent* HealthComp;

	UPROPERTY(Replicated)
	AGC_WeaponBase* CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	TSubclassOf<AGC_WeaponBase> RifleBP;

	virtual void BeginPlay() override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void PlayerCrouch();

	void AimWeapon();

	void AimWeaponCancel();

	void Fire();

	void PickUpWeapon();

	UFUNCTION(Server, Reliable, WithValidation)
	void SVPickUpWeapon();

	UFUNCTION()
	void OnHealthChanged(UGC_HealthComponent* HealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	float AimingFOV;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	float AimingInterpSpeed;

	float DefaultAimFOV;

	bool PlayerIsAiming;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player")
	bool PlayerHasDied;

	UPROPERTY(EditDefaultsOnly)
	USoundCue* HurtSound;

	UPROPERTY(EditDefaultsOnly)
	USoundCue* WalkingSound;

	UPROPERTY(EditDefaultsOnly)
	USoundCue* JumpSound;

	bool isWalking;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override;

};
