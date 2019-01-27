// Fill out your copyright notice in the Description page of Project Settings.

#include "GC_GMFreeForAll.h"
#include "GC_PlayerState.h"
#include "GC_Character.h"

AGC_GMFreeForAll::AGC_GMFreeForAll()
{
	PlayerStateClass = AGC_PlayerState::StaticClass();
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;
	IsGameOverGM = false;
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
		APlayerState* PS = It->Get()->PlayerState;
		if (PS && (PS->Score >= 1))
		{
			IsGameOverGM = true;
		}
	}
	if (IsGameOverGM)
	{
		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			APlayerController* PC = Cast<APlayerController>(*It);
			if (PC)
			{
				APlayerState* initPS = PC->PlayerState;
				AGC_PlayerState* PS = Cast<AGC_PlayerState>(initPS);
				if (PS)
				{
					PS->IsGameOver = true;
					if (PS->Score >= 1)
					{
						PS->IsWinner = true;
					}
				}
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
