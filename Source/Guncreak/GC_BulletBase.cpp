// Fill out your copyright notice in the Description page of Project Settings.

#include "GC_BulletBase.h"

// Sets default values
AGC_BulletBase::AGC_BulletBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGC_BulletBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGC_BulletBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

