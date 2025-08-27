// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuUI.generated.h"
class UInventoryUI;
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
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> StartButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Player_Btn;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventoryUI> WBP_Inventory;

	UFUNCTION()
	void StartButtonClicked();
	UFUNCTION()
	void PlayerBtnClicked();
};
