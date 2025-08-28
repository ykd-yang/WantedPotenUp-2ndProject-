// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemInfo.h"
#include "Engine/GameInstance.h"
#include "Ranking/RankingData.h"
#include "BaseBallGameInstance.generated.h"


/**
 * 
 */
UCLASS()
class HOMERUNCLASHCOPY_API UBaseBallGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UBaseBallGameInstance();

protected:
	virtual void Init() override;

private:
	UPROPERTY()
	FString RankingDataFileName = TEXT("RankingData.Json");
	
	UPROPERTY(EditAnywhere)
	FString PlayerName = TEXT("Player");

	UPROPERTY()
	FRankingArray RankingData;


	UPROPERTY()
	TObjectPtr<UMaterial> WoodMat;
	UPROPERTY()
	TObjectPtr<UMaterial> TitaniumMat;
	
public:
	FString GetPlayerName();
	void SetPlayerName(const FString& NewPlayerName);

	const TArray<FRankingData>& GetRankingData();
	void UpdateRankingData(const FString& Name, const int32 Score, const int32 HitBallCnt);

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FItemInformation PlayerItemInfo;
	
	void UpdateItemInfo(int32 itemtype);
};