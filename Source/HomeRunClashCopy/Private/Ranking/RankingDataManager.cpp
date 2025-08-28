// Fill out your copyright notice in the Description page of Project Settings.


#include "Ranking/RankingDataManager.h"

#include "JsonObjectConverter.h"
const FString RankingDataManager::URL= TEXT("https://homerun-fa203-default-rtdb.firebaseio.com/"); 

FRankingArray RankingDataManager::LoadLocal(const FString& FileName)
{
	FRankingArray RankData;
	FString JsonStr;
	if (FFileHelper::LoadFileToString(JsonStr, *(FPaths::ProjectSavedDir() / FileName)))
	{
		if (!FJsonObjectConverter::JsonObjectStringToUStruct(JsonStr, &RankData, 0, 0))
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to parse JSON into FRankingDataArray"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Ranking.json not found, returning empty ranking array"));
	}
	
	return RankData;
}

void RankingDataManager::SaveLocal(const FRankingArray& Data, const FString& FileName)
{
	FString OutputString;
	FJsonObjectConverter::UStructToJsonObjectString(Data, OutputString);

	FFileHelper::SaveStringToFile(OutputString, *(FPaths::ProjectSavedDir() / FileName));
}

void RankingDataManager::AddNewDataLocal(FRankingArray& Data, const FString& Name, const int32 Score, const int32 ThrowBall, const FString& FileName)
{
	FRankingData NewData { Name, Score, ThrowBall};
	Data.Data.Push(NewData);
	
	Data.Data.Sort([](const FRankingData& A, const FRankingData& B)
	{
		if (A.ThrowBallCount == B.ThrowBallCount)
			return A.Score > B.Score;

		return A.ThrowBallCount < B.ThrowBallCount;
	});
	
	// 상위 10개만 유지
	// if (Data.Num() > 10)
	// {
	// 	Data.SetNum(10);
	// }
	
	SaveLocal(Data, FileName);
}
