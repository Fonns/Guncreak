// Fill out your copyright notice in the Description page of Project Settings.

#include "GC_WeaponBase.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

// Sets default values
AGC_WeaponBase::AGC_WeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
}

// Called when the game starts or when spawned
void AGC_WeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGC_WeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
		if (GetWorld()->LineTraceSingleByChannel(Hit, CameraLocation, TraceEnd, ECC_Visibility, QueryParams)) {

			AActor* HitActor = Hit.GetActor();

			UGameplayStatics::ApplyPointDamage(HitActor, WeaponDamage, BulletDir, Hit, GunOwner->GetInstigatorController(), this, DamageType);

			if (ImpactEffect)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
			}
		}

		DrawDebugLine(GetWorld(), CameraLocation, TraceEnd, FColor::White, false, 1.0f, 0, 1.0f);
	}
}