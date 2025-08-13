// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "HomerunClashGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class HOMERUNCLASHCOPY_API UHomerunClashGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> UInGameUI;

	UPROPERTY()
	UUserWidget* GlobalWidget;
	
	virtual void Init() override;
	
};
