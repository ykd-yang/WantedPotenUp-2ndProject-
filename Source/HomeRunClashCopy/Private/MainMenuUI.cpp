// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuUI.h"

#include "InventoryUI.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Ranking/PlayerNameUI.h"
#include "Ranking/RankingUI.h"

void UMainMenuUI::NativeConstruct()
{
	Super::NativeConstruct();
	StartButton->OnClicked.AddDynamic(this, &UMainMenuUI::StartButtonClicked);
	Player_Btn->OnClicked.AddDynamic(this, &UMainMenuUI::PlayerBtnClicked);
	Shop_Btn->OnClicked.AddDynamic(this, &UMainMenuUI::ShopBtnClicked);
	Rank_Btn->OnClicked.AddDynamic(this, &UMainMenuUI::RankingBtnClicked);
}

void UMainMenuUI::StartButtonClicked()
{
	UGameplayStatics::OpenLevel(this, FName("BaseballLevel"));
}

void UMainMenuUI::PlayerBtnClicked()
{
	WBP_Inventory->SetVisibility(ESlateVisibility::Hidden);
	WBP_Ranking->SetVisibility(ESlateVisibility::Hidden);
	
	WBP_PlayerName->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	WBP_PlayerName->OnClickPlayerBtn();	
}

void UMainMenuUI::ShopBtnClicked()
{
	WBP_Ranking->SetVisibility(ESlateVisibility::Hidden);
	WBP_PlayerName->HidePlayerBtn();
	
	if (WBP_Inventory->GetVisibility() == ESlateVisibility::Hidden)
	{
		WBP_Inventory->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else if (WBP_Inventory->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
	{
		WBP_Inventory->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UMainMenuUI::RankingBtnClicked()
{
	WBP_Inventory->SetVisibility(ESlateVisibility::Hidden);
	WBP_PlayerName->HidePlayerBtn();
	
	if (WBP_Ranking->GetVisibility() == ESlateVisibility::Hidden)
	{
		WBP_Ranking->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else if (WBP_Ranking->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
	{
		WBP_Ranking->SetVisibility(ESlateVisibility::Hidden);
	}
}
