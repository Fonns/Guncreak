// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GC_PlayerState.generated.h"

/**
 * 
 */
UCLASS()
class GUNCREAK_API AGC_PlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void AddScore(float ScoreDelta);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsWinner = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsGameOver = false;
};
