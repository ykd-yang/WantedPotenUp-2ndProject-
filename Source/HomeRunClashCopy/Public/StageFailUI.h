// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StageFailUI.generated.h"

class UImage;
class UUserWidget;
/**
 * 
 */
UCLASS()
class HOMERUNCLASHCOPY_API UStageFailUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
public:
	// End Stage UI
	UPROPERTY(meta = (BindWidget))
	UImage* IMG_TurnStageFail;
	UPROPERTY(meta = (BindWidget))
	UImage* IMG_TurnStageFailBG;
	UPROPERTY(meta = (BindWidget))
	UImage* IMG_StageFail;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* TurnStageFailAnimation;
	// OST
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Sound")
	UAudioComponent* FailOSTComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound")
	USoundBase* FailOSTSound;

	UFUNCTION()
	void PlayFailOST();
	
	// Exit Button
	UPROPERTY(meta = (BindWidget))
	UUserWidget* WBP_ExitButton;
};
