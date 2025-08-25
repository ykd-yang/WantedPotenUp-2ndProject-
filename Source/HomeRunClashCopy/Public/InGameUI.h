// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameUI.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayAnimation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayOST);


class UProgressBar;
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
	UPROPERTY(BlueprintAssignable)
	FPlayAnimation PlayClearAnim;
	UPROPERTY(BlueprintAssignable)
	FPlayAnimation PlayFailAnim;

	UPROPERTY(BlueprintAssignable)
	FPlayOST PlayClearOST;
	UPROPERTY(BlueprintAssignable)
	FPlayOST PlayFailOST;
	
	UPROPERTY()
	ABaseBallGameMode* GameMode;


	// Homerun Gauge Text
	UPROPERTY(meta = (BindWidget))
	UTextBlock* HomerunGaugeText;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HomerunGaugeBar;
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
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* MissAnimation;
	UPROPERTY(meta = (BindWidget))
	UImage* GoodImage;
	UPROPERTY(meta = (BindWidget))
	UImage* GreatImage;
	UPROPERTY(meta = (BindWidget))
	UImage* PerfectImage;
	// Homerun State
	UPROPERTY(meta = (BindWidget))
	UImage* HomerunImage;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* HomerunAnimation;
	UPROPERTY(meta = (BindWidget))
	UImage* HitImage;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* HitAnimation;
	// Hit Distance
	UPROPERTY(meta = (BindWidget))
	UTextBlock* HitDistanceText;
	FVector StrikeZoneLocation;
	UPROPERTY()
	UCanvasPanelSlot* DistanceCanvasSlot;
	// Cycling Homerun
	UPROPERTY(meta = (BindWidget))
	UImage* LeftOnImage;
	UPROPERTY(meta = (BindWidget))
	UImage* CenterOnImage;
	UPROPERTY(meta = (BindWidget))
	UImage* RightOnImage;
	// Combo
	UPROPERTY(meta = (BindWidget))
	UOverlay* ComboOverlay;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ComboText;
	// Ready, Go
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* ReadyAnimation;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* GoAnimation;
	UPROPERTY(meta = (BindWidget))
	UImage* ReadyImage;
	UPROPERTY(meta = (BindWidget))
	UImage* ReadyLeftImage;
	UPROPERTY(meta = (BindWidget))
	UImage* ReadyRightImage;
	UPROPERTY(meta = (BindWidget))
	UImage* GoImage;
	UPROPERTY(meta = (BindWidget))
	UImage* GoDisappearImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound")
	USoundBase* PlayballSound;
	// OST
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Sound")
	UAudioComponent* InGameOSTComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound")
	USoundBase* InGameOSTSound;

	UFUNCTION(BlueprintCallable)
	void UpdateHomerunGauge(int32 NewHomerunGauge);
	UFUNCTION(BlueprintCallable)
	void ResetHomerunGauge();
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

	UFUNCTION(BlueprintCallable)
	void HideStageClear();
	UFUNCTION(BlueprintCallable)
	void HideStageFail();
	
	UFUNCTION(BlueprintCallable)
	void DisplayCyclingHomerun(FString Direction);
	UFUNCTION(BlueprintCallable)
	void HideCyclingHomerun();

	UFUNCTION(BlueprintCallable)
	void DisplayCombo();

	UFUNCTION(BlueprintCallable)
	void DisplayReady();
	UFUNCTION(BlueprintCallable)
	void HideReady();
	UFUNCTION(BlueprintCallable)
	void DisplayGo();
	UFUNCTION(BlueprintCallable)
	void HideGo();
	

	int32 ComboNumber;

	int32 HomerunGauge;

	int32 SuccessfulHomerun = 0;
	int32 IsStageCleared = -1;

	bool isJudgementDisplaying;
	bool isHomerunStateDisplaying;

	float DisplayTime = 2.5f; // UI display time
};
