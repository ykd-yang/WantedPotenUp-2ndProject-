// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseBallGameInstance.h"

#include "Ranking/RankingDataManager.h"

void UBaseBallGameInstance::Init()
{
	Super::Init();

	RankingData = RankingDataManager::Load(RankingDataFileName);
}

FString UBaseBallGameInstance::GetPlayerName()
{
	return PlayerName;
}

void UBaseBallGameInstance::SetPlayerName(const FString& NewPlayerName)
{
	PlayerName = NewPlayerName;
}

const TArray<FRankingData>& UBaseBallGameInstance::GetRankingData()
{
	return RankingData.Data;
}

void UBaseBallGameInstance::UpdateRankingData(const FString& Name, const int32 Score, const int32 HitBallCnt)
{
	RankingDataManager::AddNewData(RankingData, Name, Score, HitBallCnt, RankingDataFileName);
}
