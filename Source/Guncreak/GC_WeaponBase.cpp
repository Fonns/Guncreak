// Fill out your copyright notice in the Description page of Project Settings.

#include "GC_WeaponBase.h"
#include "GC_BulletBase.h"

// Sets default values
AGC_WeaponBase::AGC_WeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

	UE_LOG(LogTemp, Warning, TEXT("Shooting a guuunnnn!!!"));
	TArray<USkeletalMeshComponent*> SkeletalMeshComponents;
	UE_LOG(LogTemp, Warning, TEXT("Criou variavel componentes"));
	this->GetComponents<USkeletalMeshComponent>(SkeletalMeshComponents);
	UE_LOG(LogTemp, Warning, TEXT("Apanhou os componentes!!!"));
	for (int i = 0; i < SkeletalMeshComponents.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("O loop esta a funcinoaer!!!"));
		SkeletalMeshComponent = SkeletalMeshComponents[i];
	}
	UE_LOG(LogTemp, Warning, TEXT("Agr vai dar spawn a bola!!!"));
	GetWorld()->SpawnActor<AGC_BulletBase>(BulletBP, SkeletalMeshComponent->GetSocketTransform("Muzzle"));
	UE_LOG(LogTemp, Warning, TEXT("YEY fez spawn!!!"));
}