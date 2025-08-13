// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HomerunUI.generated.h"

/**
 * 
 */
class UTextBlock;

UCLASS()
class HOMERUNCLASHCOPY_API UHomerunUI : public UUserWidget
{
	GENERATED_BODY()

public:
	// declaring widget
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HomerunGaugeText;

	void UpdateHomerunGaugeText(FString newText);
	
};
