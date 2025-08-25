// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StageClearUI.generated.h"

class UUserWidget;
/**
 * 
 */
UCLASS()
class HOMERUNCLASHCOPY_API UStageClearUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Sound")
	UAudioComponent* ClearOSTComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound")
	USoundBase* ClearOSTSound;

	UFUNCTION()
	void PlayClearOST();
	
	// Exit Button
	UPROPERTY(meta = (BindWidget))
	UUserWidget* WBP_ExitButton;
};



