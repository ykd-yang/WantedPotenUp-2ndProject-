// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameUI.generated.h"

/**
 * 
 */

class UCanvasPanelSlot;
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
	// Ball Jugdement
	UPROPERTY(meta = (BindWidget))
	UImage* MissImage;
	// Homerun State
	UPROPERTY(meta = (BindWidget))
	UImage* HomerunImage;
	UPROPERTY(meta = (BindWidget))
	UImage* HitImage;
	// Hit Distance
	UPROPERTY(meta = (BindWidget))
	UTextBlock* HitDistanceText;
	FVector StrikeZoneLocation;
	UCanvasPanelSlot* DistanceCanvasSlot;
	
	
	UFUNCTION(BlueprintCallable)
	void UpdateHomerunGaugeText(float HomerunGauge);
	// 미션, 남은 공
	UFUNCTION(BlueprintCallable)
	void DeductRemainingBalls();
	UFUNCTION(BlueprintCallable)
	void UpdateSuccessfulHomerun();
	// 공의 정보!!
	UFUNCTION(BlueprintCallable)
	void DisplayBallInfo(FString BallType);
	UFUNCTION(BlueprintCallable)
	void HideBallInfo();
	// 공의 방향
	UFUNCTION(BlueprintCallable)
	void DisplayBallHitDirection(float BallHitDirection);
	UFUNCTION(BlueprintCallable)
	void HideBallHitDirection();
	// 공의 판정!!
	UFUNCTION(BlueprintCallable)
	void DisplayBallJudgement(float Judgement, bool isCritical);
	UFUNCTION(BlueprintCallable)
	void HideBallJudgement();
	UFUNCTION(BlueprintCallable)
	void DisplayMiss();
	// 비거리!!
	UFUNCTION(BlueprintCallable)
	void UpdateBallDistance(ABall* ball, APlayerController* playercontroller);
	UFUNCTION(BlueprintCallable)
	void HideBallDistance();
	bool bHidingBallDistance = false;
	
	// 홈런이냐 아니냐
	UFUNCTION(BlueprintCallable)
	void DisplayHomerunState(bool Homerun);
	UFUNCTION(BlueprintCallable)
	void HideHomerunState();
	
	int32 SuccessfulHomerun = 0;
	bool IsStageCleared;

	bool isJudgementDisplaying;
	bool isHomerunStateDisplaying;
	
	float DisplayTime = 2.5f;	// UI display time
	

};
