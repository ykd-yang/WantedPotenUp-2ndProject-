// Fill out your copyright notice in the Description page of Project Settings.


#include "Ranking/RankingDataUI.h"

#include "Components/TextBlock.h"
#include "Ranking/RankingData.h"

void URankingDataUI::NativeConstruct()
{
	Super::NativeConstruct();

	SetPadding(FMargin(0, 3, 0, 3));
}

void URankingDataUI::SetUIRankText(int32 Rank, FRankingData Data)
{
	RankText->SetText(FText::AsNumber(Rank));
	PlayerNameText->SetText(FText::FromString(Data.Name));
	ThrowBallCountText->SetText(FText::AsNumber(Data.ThrowBallCount));
	ScoreText->SetText(FText::AsNumber(Data.Score));
}
