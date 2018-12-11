// Fill out your copyright notice in the Description page of Project Settings.

#include "GC_WeaponBase.h"

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

