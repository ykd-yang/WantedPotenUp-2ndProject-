// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseBallGameInstance.h"
#include "ItemInfo.h"
#include "Ranking/RankingDataManager.h"

void UBaseBallGameInstance::Init()
{
	Super::Init();
	
	//RankingData = RankingDataManager::LoadLocal(RankingDataFileName);
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
	RankingDataManager::AddNewDataLocal(RankingData, Name, Score, HitBallCnt, RankingDataFileName);
}

void UBaseBallGameInstance::UpdateItemInfo()
{
	if (ItemType == 1)
	{
		ItemInfo.Material = ItemMaterial1->GetDefaultObject<UMaterial>();
		ItemInfo.PowerRate = 0;
		ItemInfo.CriticalRate = 0.05;
	}
	else if (ItemType == 2)
	{
		ItemInfo.Material = ItemMaterial2->GetDefaultObject<UMaterial>();
		ItemInfo.PowerRate = 300;
		ItemInfo.CriticalRate = 0.f;
	}
	
}
