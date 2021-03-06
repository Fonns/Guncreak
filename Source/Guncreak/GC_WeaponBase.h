// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GC_WeaponBase.generated.h"

class USkeletalMeshComponent;
class UDamageType;
class UParticleSystem;
class USoundCue;

USTRUCT()
struct FBulletTraceScan {

	GENERATED_BODY()

public:

	UPROPERTY()
	FVector_NetQuantize TraceTo;
};

UCLASS()
class GUNCREAK_API AGC_WeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGC_WeaponBase();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* ImpactEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<UCameraShake> FireCamShake;

	UPROPERTY(EditAnywhere, Category = "Weapon Stats")
	int WeaponRange;

	UPROPERTY(EditAnywhere, Category = "Weapon Stats")
	float WeaponDamage;

	UPROPERTY(ReplicatedUsing = OnRep_HitScanTrace)
	FBulletTraceScan HitScanTrace;

	UFUNCTION()
	void OnRep_HitScanTrace();

	void PlayImpactEffect(FVector ImpactPoint);

	UPROPERTY(EditAnywhere)
	USoundCue* FireSound;

public:	

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Fire();

	UFUNCTION(Server, Reliable, WithValidation)
	void SVFire();

};
