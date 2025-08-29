// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseBallGameInstance.h"
#include "ItemInfo.h"
#include "Ranking/RankingDataManager.h"

UBaseBallGameInstance::UBaseBallGameInstance()
{
	ConstructorHelpers::FObjectFinder<UMaterial> Mat1(
		TEXT("/Script/Engine.Material'/Game/Blueprint/Ui/Widget/Material/M_GradientBlack.M_GradientBlack'"));
	if (Mat1.Succeeded())
	{
		WoodMat = Mat1.Object;
	}

	ConstructorHelpers::FObjectFinder<UMaterial> Mat2(
		TEXT("Script/Engine.Material'/Game/Blueprint/Ui/Widget/Material/M_BlueRadialFade.M_BlueRadialFade'"));
	if (Mat2.Succeeded())
	{
		TitaniumMat = Mat2.Object;
	}
}

void UBaseBallGameInstance::Init()
{
	Super::Init();
	//RankingData = RankingDataManager::LoadLocal(RankingDataFileName);
	UpdateItemInfo(1);
	RankingData = RankingDataManager::LoadLocal(RankingDataFileName);
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

void UBaseBallGameInstance::UpdateItemInfo(int32 itemtype)
{
	if (itemtype == 1) // 1번 나무 배트
	{
		PlayerItemInfo.Material = WoodMat;
		PlayerItemInfo.PowerRate = 0;
		PlayerItemInfo.CriticalRate = 0.05;
	}
	else if (itemtype == 2)	// 2번 티타늄 배트
	{
		PlayerItemInfo.Material = TitaniumMat;
		PlayerItemInfo.PowerRate = 300;
		PlayerItemInfo.CriticalRate = 0;
	}
	
}
