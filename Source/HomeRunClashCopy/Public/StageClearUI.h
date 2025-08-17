// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StageClearUI.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class HOMERUNCLASHCOPY_API UStageClearUI : public UUserWidget
{
	GENERATED_BODY()


public:
	// Exit Button
	UPROPERTY(meta = (BindWidget))
	UButton* ExitButton;
};



