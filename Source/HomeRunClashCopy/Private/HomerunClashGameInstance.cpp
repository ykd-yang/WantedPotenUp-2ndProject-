// Fill out your copyright notice in the Description page of Project Settings.


#include "HomerunClashGameInstance.h"

#include "Blueprint/UserWidget.h"

void UHomerunClashGameInstance::Init()
{
	Super::Init();

	if (UInGameUI && GetWorld())
	{
		GlobalWidget = CreateWidget<UUserWidget>(GetWorld(), UInGameUI);
		if (GlobalWidget)
		{
			GlobalWidget->AddToViewport();
		}
	}
}
