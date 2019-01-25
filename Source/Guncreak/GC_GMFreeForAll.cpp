// Fill out your copyright notice in the Description page of Project Settings.

#include "GC_GMFreeForAll.h"
#include "GC_PlayerState.h"

AGC_GMFreeForAll::AGC_GMFreeForAll()
{
	PlayerStateClass = AGC_PlayerState::StaticClass();
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;
	IsGameOver = false;
}

void AGC_GMFreeForAll::RespawnDeadPlayers()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC && PC->GetPawn() == nullptr)
		{
			RestartPlayer(PC);
		}
	}
}

void AGC_GMFreeForAll::AnyoneWin()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		APlayerState* PS = PC->PlayerState;
		if (PS && (PS->Score >= 10))
		{
			IsGameOver = true;
		}
	}
	if (IsGameOver)
	{
		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			APlayerController* PC = It->Get();
			if (PC)
			{
				
			}
		}
	}
}

void AGC_GMFreeForAll::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RespawnDeadPlayers();
	AnyoneWin();
}
