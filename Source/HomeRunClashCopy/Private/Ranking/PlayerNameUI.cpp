// Fill out your copyright notice in the Description page of Project Settings.


#include "Ranking/PlayerNameUI.h"

#include "BaseBallGameInstance.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Ranking/RankingDataManager.h"

void UPlayerNameUI::NativeConstruct()
{
	Super::NativeConstruct();

	UBaseBallGameInstance* GI = Cast<UBaseBallGameInstance>(GetGameInstance());
	PlayerNameText->SetText(FText::FromString(GI->GetPlayerName()));
	NameInputBox->SetText(FText::FromString(GI->GetPlayerName()));

	NameInputBox->OnTextChanged.AddDynamic(this, &UPlayerNameUI::OnInputTextChanged);
	SubmitButton->OnClicked.AddDynamic(this, &UPlayerNameUI::OnClickedSubmit);

	if (GI->GetPlayerName() != TEXT("Player"))
	{
		for (UWidget* Child : RootPanel->GetAllChildren())
		{
			if (Child != PlayerNameText)
			{
				Child->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}

void UPlayerNameUI::OnClickedSubmit()
{
	UBaseBallGameInstance* GI = Cast<UBaseBallGameInstance>(GetGameInstance());
	GI->SetPlayerName(NameInputBox->GetText().ToString());
	PlayerNameText->SetText(FText::FromString(GI->GetPlayerName()));
	NameInputBox->SetText(FText::FromString(GI->GetPlayerName()));

	for (UWidget* Child : RootPanel->GetAllChildren())
	{
		if (Child != PlayerNameText)
		{
			Child->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UPlayerNameUI::OnInputTextChanged(const FText& Text)
{
	FString Nick = Text.ToString();
	if (Nick.Len() > 12)
	{
		Nick = Nick.Left(12);
		NameInputBox->SetText(FText::FromString(Nick));
	}
}

void UPlayerNameUI::OnClickPlayerBtn()
{
	for (UWidget* Child : RootPanel->GetAllChildren())
	{
		if (Child != PlayerNameText)
		{
			Child->SetVisibility(ESlateVisibility::Visible);
		}
	}
}
