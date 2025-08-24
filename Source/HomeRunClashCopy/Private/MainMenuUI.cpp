// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuUI.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UMainMenuUI::NativeConstruct()
{
	Super::NativeConstruct();
	StartButton->OnClicked.AddDynamic(this, &UMainMenuUI::StartButtonClicked);
}

void UMainMenuUI::StartButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("StartButtonClicked()"))
	UGameplayStatics::OpenLevel(this, FName("BaseballLevel"));
	//GameMode->SwitchToInGameUI();
	//GameMode->ChangeState(EGameModeState::Start);
}
