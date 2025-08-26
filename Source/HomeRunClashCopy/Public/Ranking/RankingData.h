#pragma once

#include "CoreMinimal.h"
#include "RankingData.generated.h"

USTRUCT()
struct FRankingData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FString Name;
	
	UPROPERTY(EditAnywhere)
	int32 Score;
	
	UPROPERTY(EditAnywhere)
	int32 ThrowBallCount;
};

USTRUCT()
struct FRankingArray
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TArray<FRankingData> Data;
};