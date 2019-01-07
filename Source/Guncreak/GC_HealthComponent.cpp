// Fill out your copyright notice in the Description page of Project Settings.

#include "GC_HealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "GC_GMFreeForAll.h"

// Sets default values for this component's properties
UGC_HealthComponent::UGC_HealthComponent()
{
	InitialHealth = 100.0f;

	SetIsReplicated(true);
	bIsDead = false;
}


// Called when the game starts
void UGC_HealthComponent::BeginPlay()
{
	Super::BeginPlay();

	
	if (GetOwnerRole() == ROLE_Authority)
	{
		AActor* MyOwner = GetOwner();
		if (MyOwner)
		{
			MyOwner->OnTakeAnyDamage.AddDynamic(this, &UGC_HealthComponent::HandleTakeAnyDamage);
		}
	}

	Health = InitialHealth;
}

void UGC_HealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{

	if (Damage <= 0.0f || bIsDead)
	{
		return;
	}

	Health = FMath::Clamp(Health - Damage, 0.0f, InitialHealth);

	UE_LOG(LogTemp, Warning, TEXT("Health foi alteradadwawawadawd para: %s"), *FString::SanitizeFloat(Health));

	bIsDead = Health <= 0.0f;

	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);

	if (bIsDead)
	{
		AGC_GMFreeForAll* GM = Cast<AGC_GMFreeForAll>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			GM->OnActorKilled.Broadcast(GetOwner(), DamageCauser, InstigatedBy);
		}
	}
}

void UGC_HealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UGC_HealthComponent, Health);
}