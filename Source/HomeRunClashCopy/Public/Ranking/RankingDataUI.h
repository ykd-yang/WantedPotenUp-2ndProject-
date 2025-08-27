// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RankingDataUI.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class HOMERUNCLASHCOPY_API URankingDataUI : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* RankText;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* PlayerNameText;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ThrowBallCountText;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ScoreText;

public:
	void SetUIRankText(int32 Rank, FRankingData Data);
};
