// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameUI.generated.h"

/**
 * 
 */

class UTextBlock;

UCLASS()
class HOMERUNCLASHCOPY_API UInGameUI : public UUserWidget
{
	GENERATED_BODY()

public:
	// declaring widget
	UPROPERTY(meta = (BindWidget))
	UTextBlock* HomerunGaugeText;

	void UpdateHomerunGaugeText(float HomerunGauge);
};
