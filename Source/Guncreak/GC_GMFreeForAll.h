// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GC_GMFreeForAll.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnActorKilled, AActor*, VictimActor, AActor*, KillerActor, AController*, KillerController);

/**
 * 
 */
UCLASS()
class GUNCREAK_API AGC_GMFreeForAll : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AGC_GMFreeForAll();

	UPROPERTY(BlueprintAssignable)
	FOnActorKilled OnActorKilled;
};
