// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RankingUI.generated.h"

class UButton;
struct FRankingData;
class UScrollBox;
class URankingDataUI;
/**
 * 
 */
UCLASS()
class HOMERUNCLASHCOPY_API URankingUI : public UUserWidget
{
	GENERATED_BODY()
public:
	URankingUI(const FObjectInitializer& ObjectInitializer);
	
private:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY(meta=(BindWidget))
	UScrollBox* RankingList;

	UPROPERTY(meta=(BindWidget))
	UButton* RefreshButton;

	UPROPERTY()
	TSubclassOf<URankingDataUI> RankingUIClass;

	UPROPERTY()
	TArray<FRankingData> RankingData_FB;

	int RankingListCount = 30;

	UFUNCTION()
	void OnPressRefreshButton();
};
