// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StageFailUI.generated.h"

class UUserWidget;
/**
 * 
 */
UCLASS()
class HOMERUNCLASHCOPY_API UStageFailUI : public UUserWidget
{
	GENERATED_BODY()


	
public:
	// Exit Button
	UPROPERTY(meta = (BindWidget))
	UUserWidget* WBP_ExitButton;
};
