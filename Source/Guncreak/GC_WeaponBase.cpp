// Fill out your copyright notice in the Description page of Project Settings.

#include "GC_WeaponBase.h"
#include "Guncreak.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

static int32 DebugBulletTrace = 0;
FAutoConsoleVariableRef CVARDebugBulletTrace(TEXT("Guncreak.DebugBullets"), DebugBulletTrace, TEXT("Mostra LineTrace das balas"), ECVF_Cheat);

// Sets default values
AGC_WeaponBase::AGC_WeaponBase()
{

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	WeaponDamage = 20;
	WeaponRange = 5000;
}

void AGC_WeaponBase::Fire() {

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

			if (ImpactEffect)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
			}
		}

		if (DebugBulletTrace > 0)
		{
			DrawDebugLine(GetWorld(), CameraLocation, TraceEnd, FColor::White, false, 1.0f, 0, 1.0f);
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
}