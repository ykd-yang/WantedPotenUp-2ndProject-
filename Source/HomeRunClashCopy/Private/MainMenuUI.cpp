// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuUI.h"

#include "InventoryUI.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UMainMenuUI::NativeConstruct()
{
	Super::NativeConstruct();
	StartButton->OnClicked.AddDynamic(this, &UMainMenuUI::StartButtonClicked);
	Player_Btn->OnClicked.AddDynamic(this, &UMainMenuUI::PlayerBtnClicked);
}

void UMainMenuUI::StartButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("StartButtonClicked()"))
	UGameplayStatics::OpenLevel(this, FName("BaseballLevel"));
	//GameMode->SwitchToInGameUI();
	//GameMode->ChangeState(EGameModeState::Start);
}

void UMainMenuUI::PlayerBtnClicked()
{
	if (WBP_Inventory->GetVisibility() == ESlateVisibility::Hidden)
	{
		WBP_Inventory->SetVisibility(ESlateVisibility::Visible);
	}
	else if (WBP_Inventory->GetVisibility() == ESlateVisibility::Visible)
	{
		WBP_Inventory->SetVisibility(ESlateVisibility::Hidden);
	}
}
