// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseBallGameInstance.h"

#include "Ranking/RankingDataManager.h"

void UBaseBallGameInstance::Init()
{
	Super::Init();

	RankingData = RankingDataManager::LoadLocal(RankingDataFileName);

	if (!RankingDataManager::OnRankingLoaded.IsBound())
	{
		RankingDataManager::OnRankingLoaded.BindLambda([this](const TArray<FRankingData>& Data)
		{
			for (FRankingData Elem : Data)
			{
				RankingData_FB.Add(Elem);
			}

			RankingData_FB.Sort([](const FRankingData& A, const FRankingData& B)
			{
				if (A.ThrowBallCount == B.ThrowBallCount)
					return A.Score > B.Score;

				return A.ThrowBallCount < B.ThrowBallCount;
			});

			UE_LOG(LogTemp, Warning, TEXT("Ranking data loaded %d"), RankingData_FB.Num());
		});
	}

	//RankingDataManager::LoadOnline();
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
