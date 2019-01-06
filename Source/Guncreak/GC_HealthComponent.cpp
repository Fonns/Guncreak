// Fill out your copyright notice in the Description page of Project Settings.

#include "GC_HealthComponent.h"

// Sets default values for this component's properties
UGC_HealthComponent::UGC_HealthComponent()
{
	InitialHealth = 100.0f;
}


// Called when the game starts
void UGC_HealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &UGC_HealthComponent::HandleTakeAnyDamage);
	}

	Health = InitialHealth;
}

void UGC_HealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{

	if (Damage <= 0.0f)
	{
		return;
	}

	Health = FMath::Clamp(Health - Damage, 0.0f, InitialHealth);

	UE_LOG(LogTemp, Warning, TEXT("Health foi alteradadwawawadawd para: %s"), *FString::SanitizeFloat(Health));

	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);
}
