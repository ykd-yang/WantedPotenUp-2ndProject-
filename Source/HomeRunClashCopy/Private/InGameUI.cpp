// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameUI.h"
#include "BaseBallGameMode.h"
#include "StrikeZone.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Camera/CameraActor.h"
#include "Components/AudioComponent.h"
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

	if (!InGameOSTComponent)
	{
		InGameOSTComponent = NewObject<UAudioComponent>(this);
		InGameOSTComponent->bAutoActivate = false;
		InGameOSTComponent->SetSound(InGameOSTSound);
		InGameOSTComponent->RegisterComponentWithWorld(GetWorld());
	}

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
		IsStageCleared = 0;
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
		IsStageCleared = 1;
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
		PlayAnimation(MissAnimation);
		MissImage->SetVisibility(ESlateVisibility::Visible);
		FTimerHandle MissTimer;
		GetWorld()->GetTimerManager().SetTimer(
			MissTimer, [this]() { MissImage->SetVisibility(ESlateVisibility::Hidden); }, 1.4, false);
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
		bHidingBallDistance = true;
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
				PlayAnimation(HomerunAnimation);
				HomerunImage->SetVisibility(ESlateVisibility::Visible);
				GetWorld()->GetTimerManager().SetTimer(HomerunStateTimer, this, &UInGameUI::HideHomerunState,
				                                       DisplayTime, false);
				UpdateSuccessfulHomerun();
			}
			else // Display Hit
			{
				ComboNumber = 0;
				PlayAnimation(HitAnimation);
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

	if (0 == IsStageCleared) // Stage Fail
	{
		// StageFailImage->SetVisibility(ESlateVisibility::Visible); !!
		FTimerHandle StageFailTimer;
		GetWorld()->GetTimerManager().SetTimer(StageFailTimer, this, &UInGameUI::HideStageFail, 2, false);
	}
	else if (1 == IsStageCleared) // Stage Clear
	{
		// StageClearImage->SetVisibility(ESlateVisibility::Visible); !!
		FTimerHandle StageClearTimer;
		GetWorld()->GetTimerManager().SetTimer(StageClearTimer, this, &UInGameUI::HideStageClear, 2, false);
	}
	else
	{
		FTimerHandle HideHomerunStateTimer;
		GetWorld()->GetTimerManager().SetTimer(HideHomerunStateTimer,
		                                       [this]() { GameMode->ChangeState(EGameModeState::Throw); }, 1, false);
	}
}

void UInGameUI::HideStageClear()
{
	this->SetVisibility(ESlateVisibility::Hidden);
	// StageClearImage->SetVisibility(ESlateVisibility::Hidden); !!

	// Play Clear Animation
	if (PlayClearAnim.IsBound())
	{
		PlayClearAnim.Broadcast();
	}
	// Play Clear OST !!
	InGameOSTComponent->SetVolumeMultiplier(0.15);
	FTimerHandle StopOSTTimer;
	GetWorld()->GetTimerManager().SetTimer(StopOSTTimer,
	                                       [this]() { InGameOSTComponent->Stop(); },
	                                       2,
	                                       false);
	if (PlayClearOST.IsBound())
	{
		PlayClearOST.Broadcast();
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	APawn* ClearPawn = GetWorld()->SpawnActor<APawn>(StageClearPawn, SpawnLocation, FRotator(0,-90.f,0), Params);
	if (ClearPawn)
	{
		ClearPawn->SetActorScale3D(FVector(3.0f, 3.0f, 3.0f));
	}
	// Play Camera Move !!

	GameMode->ChangeState(EGameModeState::End); // play UI after camera move
}

void UInGameUI::HideStageFail()
{
	this->SetVisibility(ESlateVisibility::Hidden);
	// StageFailImage->SetVisibility(ESlateVisibility::Hidden); !!

	// Play Fail Animation
	if (PlayFailAnim.IsBound())
	{
		PlayFailAnim.Broadcast();
	}
	// Play Fail OST !!
	InGameOSTComponent->SetVolumeMultiplier(0.15);
	FTimerHandle StopOSTTimer;
	GetWorld()->GetTimerManager().SetTimer(StopOSTTimer,
	                                       [this]() { InGameOSTComponent->Stop(); },
	                                       2,
	                                       false);
	if (PlayFailOST.IsBound())
	{
		PlayFailOST.Broadcast();
	}
	
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	APawn* FailPawn = GetWorld()->SpawnActor<APawn>(StageFailPawn, SpawnLocation, FRotator(0,-90.f,0), Params);
	if (FailPawn)
	{
		FailPawn->SetActorScale3D(FVector(3.0f, 3.0f, 3.0f));
	}
	// Play Camera Move !!
	GameMode->ChangeState(EGameModeState::End); // play UI after camera move
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
	GetWorld()->GetTimerManager().SetTimer(HideCyclingHomerunTimer, this, &UInGameUI::HideCyclingHomerun, DisplayTime,
	                                       false);
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
	ReadyLeftImage->SetVisibility(ESlateVisibility::Visible);
	ReadyRightImage->SetVisibility(ESlateVisibility::Visible);
	ReadyImage->SetVisibility(ESlateVisibility::Visible);
	PlayAnimation(ReadyAnimation);

	FTimerHandle DisplayReadyTimer;
	GetWorld()->GetTimerManager().SetTimer(DisplayReadyTimer, this, &UInGameUI::HideReady, 1.75f, false);
}

void UInGameUI::HideReady()
{
	ReadyLeftImage->SetVisibility(ESlateVisibility::Hidden);
	ReadyRightImage->SetVisibility(ESlateVisibility::Hidden);
	ReadyImage->SetVisibility(ESlateVisibility::Hidden);

	InGameOSTComponent->Play();
	InGameOSTComponent->SetVolumeMultiplier(0.15);

	FTimerHandle HideReadyTimer;
	GetWorld()->GetTimerManager().SetTimer(HideReadyTimer, this, &UInGameUI::DisplayGo, 2.35f, false);
}

void UInGameUI::DisplayGo()
{
	GoDisappearImage->SetVisibility(ESlateVisibility::Visible);
	GoImage->SetVisibility(ESlateVisibility::Visible);
	PlayAnimation(GoAnimation);

	UGameplayStatics::PlaySound2D(GetWorld(), PlayballSound);

	FTimerHandle DisplayGoTimer;
	GetWorld()->GetTimerManager().SetTimer(DisplayGoTimer, this, &UInGameUI::HideGo, 1.16f, false);
}

void UInGameUI::HideGo()
{
	InGameOSTComponent->SetVolumeMultiplier(0.55);

	GoDisappearImage->SetVisibility(ESlateVisibility::Hidden);
	GoImage->SetVisibility(ESlateVisibility::Hidden);
}
