// Fill out your copyright notice in the Description page of Project Settings.


#include "HomerunUI.h"

#include "Components/TextBlock.h"

void UHomerunUI::UpdateHomerunGaugeText(FString newText)
{
	HomerunGaugeText->SetText(FText::FromString(newText));
}
