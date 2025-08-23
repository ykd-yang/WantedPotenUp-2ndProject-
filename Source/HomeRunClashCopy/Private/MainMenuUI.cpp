// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuUI.h"

#include "BaseBallGameMode.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UMainMenuUI::NativeConstruct()
{
	Super::NativeConstruct();

	// Get GameMode (to get variables)
	GameMode = Cast<ABaseBallGameMode>(UGameplayStatics::GetGameMode(GetWorld()));


	StartButton->OnClicked.AddDynamic(this, &UMainMenuUI::StartButtonClicked);
}

void UMainMenuUI::StartButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("StartButtonClicked()"))
	GameMode->SwitchToInGameUI();
	GameMode->ChangeState(EGameModeState::Start);
}
