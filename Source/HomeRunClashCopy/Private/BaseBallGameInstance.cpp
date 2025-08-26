// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseBallGameInstance.h"

#include "Ranking/RankingDataManager.h"

void UBaseBallGameInstance::Init()
{
	Super::Init();

	RankingData = RankingDataManager::Load(RankingDataFileName);
	RankingDataManager::AddNewData(RankingData, TEXT("Me"), 1, 1, RankingDataFileName);
}

FString UBaseBallGameInstance::GetPlayerName()
{
	return PlayerName;
}

void UBaseBallGameInstance::SetPlayerName(const FString& NewPlayerName)
{
	PlayerName = NewPlayerName;
}
