// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameUI.h"
#include "BaseBallGameMode.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"


// InGameUI BeginPlay
void UInGameUI::NativeConstruct()
{
	Super::NativeConstruct();

	// Get GameMode (to get variables)
	GameMode = Cast<ABaseBallGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	// Initializing remaining ball count on UI.
	RemainingBallText->SetText(FText::AsNumber(GameMode->RemainingBalls));

	// Initializing homerun amounts needed to clear stage.
	FText WinConditionText = FText::FromString(TEXT("0/{0}"));
	int32 WinConditionInt = GameMode->HomerunsForWin;
	FText WinCondition = FText::Format(WinConditionText, FText::AsNumber(WinConditionInt));
	MainMissionCounterText->SetText(WinCondition);
}


// InGameUI Tick
void UInGameUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}


// Update Homerun Gauge 구현 필요
void UInGameUI::UpdateHomerunGaugeText(float HomerunGauge)
{
	int32 HomerunGaugeInt = static_cast<int>(HomerunGauge);
	FString HomerunGaugeString = FString::FromInt(HomerunGaugeInt);
	
	HomerunGaugeText->SetText(FText::FromString(HomerunGaugeString + "%"));
}


// Deduct Remaining Ball Count
void UInGameUI::DeductRemainingBalls()
{
	FString RemainingBallsString = RemainingBallText->GetText().ToString();
	int32 RemainingBallsInt = FCString::Atoi(*RemainingBallsString) - 1;
	RemainingBallText->SetText(FText::AsNumber(RemainingBallsInt));	// Deduct remaining balls.
	
	// Stage failed. (no more remaining balls)
	if (GameMode->RemainingBalls <= 0 && GameMode->HomerunsForWin > SuccessfulHomerun)
	{
		IsStageCleared = false;
		GameMode->ChangeState(EGameModeState::End);
		
	}
}


// Update Main Mission UI
void UInGameUI::UpdateSuccessfulHomerun()
{
	FText SuccessfulHomerunText = FText::FromString(TEXT("{0}/{1}"));
	SuccessfulHomerun += 1;
	SuccessfulHomerunText = FText::Format(SuccessfulHomerunText,FText::AsNumber(SuccessfulHomerun) , FText::AsNumber(GameMode->HomerunsForWin));
	MainMissionCounterText->SetText(SuccessfulHomerunText);	// Count after a successful homerun.
	
	// Stage cleared.
	if (GameMode->HomerunsForWin <= SuccessfulHomerun)	// is there more successful homerun than stage required homerun?
	{
		IsStageCleared = true;
		GameMode->ChangeState(EGameModeState::End);
	}
}


// Display Ball Info
void UInGameUI::DisplayBallInfo(FString BallType)
{
	// 입력 구종에 따라 구종표시 및 구종에 따른 랜덤 속력

	
	BallInfoOverlay->SetVisibility(ESlateVisibility::Visible);
	PlayAnimation(BallInfoAnimation,0.f,1, EUMGSequencePlayMode::Forward,1.f);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UInGameUI::HideBallInfo, DisplayTime, false);
}
// Hide Ball Info
void UInGameUI::HideBallInfo()
{
	BallInfoOverlay->SetVisibility(ESlateVisibility::Hidden);
	GameMode->ChangeState(EGameModeState::Throw);
}


// Display Ball Direction and Indicator
void UInGameUI::DisplayBallHitDirection(float BallHitDirection)
{
	// No Ball Direction Input -> Display Only HitDirection
	if (-1 > BallHitDirection)
	{
		// Display Ball Direction
		HitDirectionOverlay->SetVisibility(ESlateVisibility::Visible);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UInGameUI::HideBallHitDirection, DisplayTime, false);
		return;
	}
	float PositionX = FMath::GetMappedRangeValueClamped(FVector2D(-1.0f, 1.0f),FVector2D(-130.0f, 130.0f), BallHitDirection);	// Position X: -130 ~ 130
	UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(HitIndicatorUI);	// Set Hit Indicator Position to Ball Direction
	FVector2D CanvasPosition = CanvasSlot->GetPosition();
	CanvasPosition.X = PositionX;
	CanvasSlot->SetPosition(CanvasPosition);
	// Display Ball Direction & Indicator
	HitIndicatorUI->SetVisibility(ESlateVisibility::Visible);
	HitDirectionOverlay->SetVisibility(ESlateVisibility::Visible);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UInGameUI::HideBallHitDirection, DisplayTime, false);
}
// Hide Ball Direction and Indicator
void UInGameUI::HideBallHitDirection()
{
	HitIndicatorUI->SetVisibility(ESlateVisibility::Hidden);
	HitDirectionOverlay->SetVisibility(ESlateVisibility::Hidden);
}


// Display Ball Judgement UI
void UInGameUI::DisplayBallJudgement(int32 Judgement)	
{
	isJudgementDisplaying = true;
}

void UInGameUI::HideBallJudgement()
{
	
	isJudgementDisplaying = false;
}
// Display Miss UI
void UInGameUI::DisplayMiss()
{
	MissImage->SetVisibility(ESlateVisibility::Visible);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle,[this](){MissImage->SetVisibility(ESlateVisibility::Hidden);},DisplayTime,false);
}


// Display Ball Distance
void UInGameUI::UpdateBallDistance(float Distance)
{
	
}


// Display Homerun State
void UInGameUI::DisplayHomerunState(bool Homerun)
{
	isHomerunStateDisplaying = true;
	if (Homerun)	// Display Homerun
	{
		HitIndicatorUI->SetVisibility(ESlateVisibility::Visible);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UInGameUI::HideHomerunState, DisplayTime, false);

	}
	else	// Display Hit
	{
		HitIndicatorUI->SetVisibility(ESlateVisibility::Visible);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UInGameUI::HideHomerunState, DisplayTime, false);

	}
}
// Hide Homerun State
void UInGameUI::HideHomerunState()
{
	HitIndicatorUI->SetVisibility(ESlateVisibility::Hidden);
	HitIndicatorUI->SetVisibility(ESlateVisibility::Hidden);
	UpdateSuccessfulHomerun();
	DeductRemainingBalls();
	GetWorld()->GetTimerManager().SetTimer(TimerHandle,[this](){GameMode->ChangeState(EGameModeState::Throw);},1,false);
}

