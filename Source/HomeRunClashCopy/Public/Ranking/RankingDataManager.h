// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RankingData.h"

/**
 * 
 */
class HOMERUNCLASHCOPY_API RankingDataManager
{
public:
	const static FString URL;
	
public:
	static FRankingArray LoadLocal(const FString& FileName);
	static void SaveLocal(const FRankingArray& Data, const FString& FileName);
	static void AddNewDataLocal(FRankingArray& Data, const FString& Name, const int32 Score, const int32 ThrowBall, const FString& FileName);
};
