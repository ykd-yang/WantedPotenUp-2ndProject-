// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameUI.generated.h"

/**
 * 
 */

class ABaseBallGameMode;
class UTextBlock;
class UImage;
class UOverlay;

UCLASS()
class HOMERUNCLASHCOPY_API UInGameUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
public:
	UPROPERTY()
	ABaseBallGameMode* GameMode;
	
	// Homerun Gauge Text
	UPROPERTY(meta = (BindWidget))
	UTextBlock* HomerunGaugeText;
	// Main Mission Counter & Remaining Balls
	UPROPERTY(meta = (BindWidget))
	UTextBlock* RemainingBallText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MainMissionCounterText;
	// Hit Direction & Indicator
	UPROPERTY(meta = (BindWidget))
	UOverlay* HitDirectionOverlay;
	UPROPERTY(meta = (BindWidget))
	UImage* HitIndicatorUI;
	// Ball Info & Animation
	UPROPERTY(meta = (BindWidget))
	UOverlay* BallInfoOverlay;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* BallInfoAnimation;
	
	UFUNCTION(BlueprintCallable)
	void UpdateHomerunGaugeText(float HomerunGauge);
	UFUNCTION(BlueprintCallable)
	void DeductRemainingBalls();
	UFUNCTION(BlueprintCallable)
	void UpdateSuccessfulHomerun();
	UFUNCTION(BlueprintCallable)
	void DisplayBallInfo(float BallSpeed);	// 나중에 구현
	UFUNCTION(BlueprintCallable)
	void DisplayBallHitDirection(float BallHitDirection);
	UFUNCTION(BlueprintCallable)
	void HideBallHitDirection();
	
	int32 SuccessfulHomerun = 0;
	bool IsDirectionDisplaying = false;
};
