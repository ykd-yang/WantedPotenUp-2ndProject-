// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RankingUI.generated.h"

class UScrollBox;
/**
 * 
 */
UCLASS()
class HOMERUNCLASHCOPY_API URankingUI : public UUserWidget
{
	GENERATED_BODY()
private:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY(meta=(BindWidget))
	UScrollBox* RankingList;

	UPROPERTY(meta=(BindWidget))
	UButton* ExitButton;
};
