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
	static FRankingArray Load(const FString& FileName);
	static void Save(const FRankingArray& Data, const FString& FileName);
	static void AddNewData(FRankingArray& Data, const FString& Name, const int32 Score, const int32 ThrowBall, const FString& FileName);
};
