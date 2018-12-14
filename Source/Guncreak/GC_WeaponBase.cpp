// Fill out your copyright notice in the Description page of Project Settings.

#include "GC_WeaponBase.h"
#include "Guncreak.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Net/UnrealNetwork.h"

static int32 DebugBulletTrace = 0;
FAutoConsoleVariableRef CVARDebugBulletTrace(TEXT("Guncreak.DebugBullets"), DebugBulletTrace, TEXT("Mostra LineTrace das balas"), ECVF_Cheat);

// Sets default values
AGC_WeaponBase::AGC_WeaponBase()
{

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	WeaponDamage = 20;
	WeaponRange = 5000;

	SetReplicates(true);
}

void AGC_WeaponBase::Fire() {

	if (Role < ROLE_Authority)
	{
		SVFire();
	}

	AActor* GunOwner = GetOwner();
	if (GunOwner)
	{
		FVector CameraLocation;
		FRotator CameraRotation;
		GunOwner->GetActorEyesViewPoint(CameraLocation, CameraRotation);

		FVector BulletDir = CameraRotation.Vector();
		FVector TraceEnd = CameraLocation + (BulletDir * WeaponRange);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(GunOwner);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;

		FHitResult Hit;
		if (GetWorld()->LineTraceSingleByChannel(Hit, CameraLocation, TraceEnd, COLLISION_BULLETS, QueryParams)) {

			AActor* HitActor = Hit.GetActor();

			UGameplayStatics::ApplyPointDamage(HitActor, WeaponDamage, BulletDir, Hit, GunOwner->GetInstigatorController(), this, DamageType);

			PlayImpactEffect(Hit.ImpactPoint);

		}

		if (DebugBulletTrace > 0)
		{
			DrawDebugLine(GetWorld(), CameraLocation, TraceEnd, FColor::White, false, 1.0f, 0, 1.0f);
		}

		if (Role == ROLE_Authority)
		{
			UE_LOG(LogTemp, Warning, TEXT("start onrep"));
			BulletTraceScan.BulletEnd = TraceEnd;
		}
	}
}

void AGC_WeaponBase::SVFire_Implementation() {

	Fire();
}

bool AGC_WeaponBase::SVFire_Validate() {

	return true;
}

void AGC_WeaponBase::OnRepBulletTraceScan()
{
	UE_LOG(LogTemp, Warning, TEXT("onrepping"));
	PlayImpactEffect(BulletTraceScan.BulletEnd);
}

void AGC_WeaponBase::PlayImpactEffect(FVector ImpactPoint)
{

	if (ImpactEffect)
	{
		FVector BulletDirection = ImpactPoint - (MeshComponent->GetSocketLocation("RifleMuzzle"));
		BulletDirection.Normalize();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, ImpactPoint, BulletDirection.Rotation());
	}

	APawn* GunOwner = Cast<APawn>(GetOwner());
	if (GunOwner)
	{
		APlayerController* PlayerController = Cast<APlayerController>(GunOwner->GetController());
		if (PlayerController)
		{
			PlayerController->ClientPlayCameraShake(FireCamShake);
		}
	}
}

void AGC_WeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AGC_WeaponBase, BulletTraceScan, COND_SkipOwner);
}