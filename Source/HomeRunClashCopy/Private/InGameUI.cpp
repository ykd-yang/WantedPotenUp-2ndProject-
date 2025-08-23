// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameUI.h"
#include "BaseBallGameMode.h"
#include "StrikeZone.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"


// InGameUI BeginPlay
void UInGameUI::NativeConstruct()
{
	Super::NativeConstruct();

	// Get GameMode (to get variables)
	GameMode = Cast<ABaseBallGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	AStrikeZone* StrikeZone = Cast<AStrikeZone>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AStrikeZone::StaticClass()));
	StrikeZoneLocation = StrikeZone->GetActorLocation();
	// Initializing remaining ball count on UI.
	RemainingBallText->SetText(FText::AsNumber(GameMode->RemainingBalls));

	// Initializing homerun amounts needed to clear stage.
	FText WinConditionText = FText::FromString(TEXT("0/{0}"));
	int32 WinConditionInt = GameMode->HomerunsForWin;
	FText WinCondition = FText::Format(WinConditionText, FText::AsNumber(WinConditionInt));
	MainMissionCounterText->SetText(WinCondition);

	DistanceCanvasSlot = Cast<UCanvasPanelSlot>(HitDistanceText->Slot);

	ResetHomerunGauge();
}

// InGameUI Tick
void UInGameUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}


// Update Homerun Gauge
void UInGameUI::UpdateHomerunGauge(int32 NewHomerunGauge)
{
	HomerunGauge += NewHomerunGauge;
	if (100 <= HomerunGauge) HomerunGauge = 100; // over 100
	HomerunGaugeText->SetText(FText::Format(NSLOCTEXT("UI", "HomerunGauge", "{0}%"), FText::AsNumber(HomerunGauge)));
	HomerunGaugeBar->SetPercent(HomerunGauge / 100.0f);
}

void UInGameUI::ResetHomerunGauge()
{
	HomerunGauge = 0;
	HomerunGaugeText->SetText(FText::Format(NSLOCTEXT("UI", "HomerunGauge", "{0}%"), FText::AsNumber(HomerunGauge)));
	HomerunGaugeBar->SetPercent(HomerunGauge);
}


// Deduct Remaining Ball Count
void UInGameUI::DeductRemainingBalls()
{
	FString RemainingBallsString = RemainingBallText->GetText().ToString();
	int32 RemainingBallsInt = FCString::Atoi(*RemainingBallsString) - 1;
	RemainingBallText->SetText(FText::AsNumber(RemainingBallsInt)); // Deduct remaining balls.

	// Stage failed. (no more remaining balls)
	if (RemainingBallsInt <= 0 && GameMode->HomerunsForWin > SuccessfulHomerun)
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
	SuccessfulHomerunText = FText::Format(SuccessfulHomerunText, FText::AsNumber(SuccessfulHomerun),
	                                      FText::AsNumber(GameMode->HomerunsForWin));
	MainMissionCounterText->SetText(SuccessfulHomerunText); // Count after a successful homerun.

	// Stage cleared.
	if (GameMode->HomerunsForWin <= SuccessfulHomerun)
	// is there more successful homerun than stage required homerun?
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
	PlayAnimation(BallInfoAnimation, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f);
	FTimerHandle BallInfoTimer;
	GetWorld()->GetTimerManager().SetTimer(BallInfoTimer, this, &UInGameUI::HideBallInfo, DisplayTime, false);
}

// Hide Ball Info
void UInGameUI::HideBallInfo()
{
	BallInfoOverlay->SetVisibility(ESlateVisibility::Hidden);
}


// Display Ball Direction and Indicator
void UInGameUI::DisplayBallHitDirection(float BallHitDirection)
{
	FTimerHandle HitDirectionTimer;
	// No Ball Direction Input -> Display Only HitDirection
	if (-1 > BallHitDirection)
	{
		// Display Ball Direction
		HitDirectionOverlay->SetVisibility(ESlateVisibility::Visible);
		GetWorld()->GetTimerManager().SetTimer(HitDirectionTimer, this, &UInGameUI::HideBallHitDirection,
		                                       DisplayTime,
		                                       false);
		return;
	}
	float PositionX = FMath::GetMappedRangeValueClamped(FVector2D(-1.0f, 1.0f), FVector2D(-130.0f, 130.0f),
	                                                    BallHitDirection); // Position X: -130 ~ 130
	UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(HitIndicatorUI);
	// Set Hit Indicator Position to Ball Direction
	FVector2D CanvasPosition = CanvasSlot->GetPosition();
	CanvasPosition.X = PositionX;
	CanvasSlot->SetPosition(CanvasPosition);
	// Display Ball Direction & Indicator
	HitIndicatorUI->SetVisibility(ESlateVisibility::Visible);
	HitDirectionOverlay->SetVisibility(ESlateVisibility::Visible);
	GetWorld()->GetTimerManager().SetTimer(HitDirectionTimer, this, &UInGameUI::HideBallHitDirection, DisplayTime,
	                                       false);
}

// Hide Ball Direction and Indicator
void UInGameUI::HideBallHitDirection()
{
	HitIndicatorUI->SetVisibility(ESlateVisibility::Hidden);
	HitDirectionOverlay->SetVisibility(ESlateVisibility::Hidden);
}


// Display Ball Judgement UI
void UInGameUI::DisplayBallJudgement(float Judgement, bool isCritical)
{
	isJudgementDisplaying = true;
	if (isCritical)
	{
	}
	else // Not Critical
	{
		if (Judgement > -0.33f && Judgement < 0.33f)
		{
			UpdateHomerunGauge(30);
			PerfectImage->SetVisibility(ESlateVisibility::Visible);
		}
		else if (Judgement > -0.66f && Judgement < 0.66f)
		{
			UpdateHomerunGauge(15);
			GreatImage->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			UpdateHomerunGauge(7);
			GoodImage->SetVisibility(ESlateVisibility::Visible);
		}
	}

	FTimerHandle JudgementTimer;
	GetWorld()->GetTimerManager().SetTimer(JudgementTimer, this, &UInGameUI::HideBallJudgement, DisplayTime, false);
}

void UInGameUI::HideBallJudgement()
{
	GoodImage->SetVisibility(ESlateVisibility::Hidden); // 변경 필요
	GreatImage->SetVisibility(ESlateVisibility::Hidden); // 변경 필요
	PerfectImage->SetVisibility(ESlateVisibility::Hidden); // 변경 필요
	isJudgementDisplaying = false;
}

// Display Miss UI
void UInGameUI::DisplayMiss()
{
	if (ESlateVisibility::Hidden == MissImage->GetVisibility())
	{
		MissImage->SetVisibility(ESlateVisibility::Visible);
		FTimerHandle MissTimer;
		GetWorld()->GetTimerManager().SetTimer(
			MissTimer, [this]() { MissImage->SetVisibility(ESlateVisibility::Hidden); }, DisplayTime, false);
	}
}


// Display Ball Distance
void UInGameUI::UpdateBallDistance(ABall* ball, APlayerController* playercontroller)
{
	HitDistanceText->SetVisibility(ESlateVisibility::Visible);

	if (ball && playercontroller && DistanceCanvasSlot)
	{
		FVector2D ScreenPosition;
		playercontroller->ProjectWorldLocationToScreen(ball->GetActorLocation(), ScreenPosition);
		DistanceCanvasSlot->SetPosition(ScreenPosition);

		int32 Distance = FVector::Dist(ball->GetActorLocation(), StrikeZoneLocation);
		FText WinConditionText = FText::FromString(TEXT("{0}FT"));
		FText WinCondition = FText::Format(WinConditionText, FText::AsNumber(Distance));
	}
}

void UInGameUI::HideBallDistance()
{
	if (!bHidingBallDistance)
	{
		FTimerHandle BallDistanceTimer;
		GetWorld()->GetTimerManager().SetTimer(BallDistanceTimer,
		                                       [this]()
		                                       {
			                                       HitDistanceText->SetVisibility(ESlateVisibility::Hidden);
		                                       }, 1, false);
	}
}


// Display Homerun State
void UInGameUI::DisplayHomerunState(bool Homerun)
{
	if (nullptr != this)
	{
		if (ESlateVisibility::Visible != HomerunImage->GetVisibility() && ESlateVisibility::Visible != HitImage->
			GetVisibility())
		{
			FTimerHandle HomerunStateTimer;
			isHomerunStateDisplaying = true;
			if (Homerun) // Display Homerun
			{
				ComboNumber += 1;
				HomerunImage->SetVisibility(ESlateVisibility::Visible);
				GetWorld()->GetTimerManager().SetTimer(HomerunStateTimer, this, &UInGameUI::HideHomerunState,
				                                       DisplayTime, false);
				UpdateSuccessfulHomerun();
			}
			else // Display Hit
			{
				ComboNumber = 0;
				HitImage->SetVisibility(ESlateVisibility::Visible);
				GetWorld()->GetTimerManager().SetTimer(HomerunStateTimer, this, &UInGameUI::HideHomerunState,
				                                       DisplayTime, false);
			}
		}
	}
}

// Hide Homerun State
void UInGameUI::HideHomerunState()
{
	HomerunImage->SetVisibility(ESlateVisibility::Hidden);
	HitImage->SetVisibility(ESlateVisibility::Hidden);
	DeductRemainingBalls();
	DisplayCombo();

	if (nullptr != GameMode->Ball)
	{
		GameMode->Ball->Destroy(); // Destroy Ball
	}

	FTimerHandle HideHomerunStateTimer;
	GetWorld()->GetTimerManager().SetTimer(HideHomerunStateTimer,
	                                       [this]() { GameMode->ChangeState(EGameModeState::Throw); }, 2, false);
}

void UInGameUI::DisplayCyclingHomerun(FString Direction)
{
	if (Direction == "Left")
	{
		LeftOnImage->SetVisibility(ESlateVisibility::Visible);
	}
	if (Direction == "Center")
	{
		CenterOnImage->SetVisibility(ESlateVisibility::Visible);
	}
	if (Direction == "Right")
	{
		RightOnImage->SetVisibility(ESlateVisibility::Visible);
	}

	FTimerHandle HideCyclingHomerunTimer;
	GetWorld()->GetTimerManager().SetTimer(HideCyclingHomerunTimer, this, &UInGameUI::HideCyclingHomerun, DisplayTime, false);

}

void UInGameUI::HideCyclingHomerun()
{
	if (LeftOnImage->GetVisibility() == ESlateVisibility::Visible && CenterOnImage->GetVisibility() ==
		ESlateVisibility::Visible && RightOnImage->GetVisibility() == ESlateVisibility::Visible)
	{
		LeftOnImage->SetVisibility(ESlateVisibility::Hidden);
		CenterOnImage->SetVisibility(ESlateVisibility::Hidden);
		RightOnImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UInGameUI::DisplayCombo()
{
	if (2 <= ComboNumber)
	{
		ComboText->SetText(FText::AsNumber(ComboNumber));
		ComboOverlay->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		ComboOverlay->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UInGameUI::DisplayReady()
{
	ReadyImage->SetVisibility(ESlateVisibility::Visible);
	FTimerHandle DisplayReadyTimer;
	GetWorld()->GetTimerManager().SetTimer(DisplayReadyTimer, this, &UInGameUI::HideyReady, 1.85f, false);

}

void UInGameUI::HideyReady()
{
	ReadyImage->SetVisibility(ESlateVisibility::Hidden);
	FTimerHandle HideReadyTimer;
	GetWorld()->GetTimerManager().SetTimer(HideReadyTimer, this, &UInGameUI::DisplayGo, 2.7f, false);
}

void UInGameUI::DisplayGo()
{
	GoImage->SetVisibility(ESlateVisibility::Visible);
	FTimerHandle DisplayGoTimer;
	GetWorld()->GetTimerManager().SetTimer(DisplayGoTimer, this, &UInGameUI::HideGo, 1.1, false);

}

void UInGameUI::HideGo()
{
	GoImage->SetVisibility(ESlateVisibility::Hidden);
}
