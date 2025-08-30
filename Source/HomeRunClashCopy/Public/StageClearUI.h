// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StageClearUI.generated.h"

class UImage;
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
	// End Stage UI
	UPROPERTY(meta = (BindWidget))
	UImage* IMG_TurnStageClear;
	UPROPERTY(meta = (BindWidget))
	UImage* IMG_TurnStageClearDisappear;
	UPROPERTY(meta = (BindWidget))
	UImage* IMG_TurnStageClearBG;
	UPROPERTY(meta = (BindWidget))
	UImage* IMG_StageClear;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* TurnStageClearAnimation;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* StageClearAnimation;
	// OST
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Sound")
	UAudioComponent* ClearOSTComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound")
	USoundBase* ClearOSTSound;

	UFUNCTION()
	void PlayClearOST();
	
	// Exit Button
	UPROPERTY(meta = (BindWidget))
	UUserWidget* WBP_ExitButton;
	

	void PlayStageClearAnimation();
};



