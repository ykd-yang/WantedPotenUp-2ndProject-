// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuUI.generated.h"
class ABaseBallGameMode;
class UButton;
/**
 * 
 */
UCLASS()
class HOMERUNCLASHCOPY_API UMainMenuUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY()
	ABaseBallGameMode* GameMode;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> StartButton;

	UFUNCTION()
	void StartButtonClicked();
};
