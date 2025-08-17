// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WBP_StageClearUI.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class HOMERUNCLASHCOPY_API UWBP_StageClearUI : public UUserWidget
{
	GENERATED_BODY()


public:
	// Exit Button
	UPROPERTY(meta = (BindWidget))
	UButton* ExitButton;
};



