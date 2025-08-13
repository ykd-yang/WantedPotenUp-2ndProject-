// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameUI.h"

#include "Components/TextBlock.h"


void UInGameUI::UpdateHomerunGaugeText(float HomerunGauge)
{
	int32 HomerunGaugeInt = static_cast<int>(HomerunGauge);
	FString HomerunGaugeString = FString::FromInt(HomerunGaugeInt);
	
	HomerunGaugeText->SetText(FText::FromString(HomerunGaugeString + "%"));
}
