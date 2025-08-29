// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameUI.h"

#include "BaseBallGameInstance.h"
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
#include "Ranking/RankingDataManager.h"


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
	RemainBallCount = GameMode->RemainingBalls;
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
	if (100 <= HomerunGauge)
	{
		HomerunGauge = 100; // over 100
		isCalledShot = true;
	}

	HomerunGaugeText->SetText(FText::Format(NSLOCTEXT("UI", "HomerunGauge", "{0}%"), FText::AsNumber(HomerunGauge)));
	HomerunGaugeBar->SetPercent(HomerunGauge / 100.0f);
}

void UInGameUI::ResetHomerunGauge()
{
	isCalledShot = false;
	HomerunGauge = 0;
	HomerunGaugeText->SetText(FText::Format(NSLOCTEXT("UI", "HomerunGauge", "{0}%"), FText::AsNumber(HomerunGauge)));
	HomerunGaugeBar->SetPercent(HomerunGauge);
}


// Deduct Remaining Ball Count
void UInGameUI::DeductRemainingBalls()
{
	//FString RemainingBallsString = RemainingBallText->GetText().ToString();
	//int32 RemainingBallsInt = FCString::Atoi(*RemainingBallsString) - 1;

	RemainBallCount--;
	RemainingBallText->SetText(FText::AsNumber(RemainBallCount)); // Deduct remaining balls.

	// Stage failed. (no more remaining balls)
	if (RemainBallCount <= 0 && GameMode->HomerunsForWin > SuccessfulHomerun)
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
		
		UBaseBallGameInstance* GI = Cast<UBaseBallGameInstance>(GetGameInstance());
		RankingDataManager::SaveOnline({
			GI->GetPlayerName(), GameMode->Score, GameMode->RemainingBalls - RemainBallCount + 1
		});
	}
}


// Display Ball Info
void UInGameUI::DisplayBallInfo(EBallType BallType)
{
	// 입력 구종에 따라 구종표시 및 구종에 따른 랜덤 속력
	int32 RandSpeed = 70;
	switch (BallType)
	{
	case EBallType::Straight:
		BallTypeText->SetText(FText::FromString("4SFB"));
		RandSpeed = FMath::RandRange(70, 80);
		break;
	case EBallType::Curve:
		BallTypeText->SetText(FText::FromString("CU"));
		RandSpeed = FMath::RandRange(65, 75);
		break;
	case EBallType::Fork:
		BallTypeText->SetText(FText::FromString("FO"));
		RandSpeed = FMath::RandRange(68, 78);
		break;
	case EBallType::ChangeUp:
		BallTypeText->SetText(FText::FromString("CH"));
		RandSpeed = FMath::RandRange(66, 76);
		break;
	case EBallType::Slider:
		BallTypeText->SetText(FText::FromString("SL"));
		RandSpeed = FMath::RandRange(67, 77);
		break;
	case EBallType::Knuckle:
		BallTypeText->SetText(FText::FromString("KN"));
		RandSpeed = FMath::RandRange(57, 65);
		break;
	}
	BallSpeedText->SetText(FText::Format(FText::FromString(TEXT("{0}MPH")), FText::AsNumber(RandSpeed)));
	
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
	CurrentDirection = BallHitDirection;
	// No Ball Direction Input -> Display Only HitDirection
	if (-1 > BallHitDirection)
	{
		CurrentDirection = BallHitDirection;
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
		UpdateHomerunGauge(50);
		GameMode->AddScore(180);
		GameMode->AddScore(180);
	}
	else // Not Critical
	{
		if (Judgement > -0.33f && Judgement < 0.33f)
		{
			UpdateHomerunGauge(30);
			PerfectImage->SetVisibility(ESlateVisibility::Visible);
			GameMode->AddScore(140);
		}
		else if (Judgement > -0.66f && Judgement < 0.66f)
		{
			UpdateHomerunGauge(15);
			GreatImage->SetVisibility(ESlateVisibility::Visible);
			GameMode->AddScore(80);
		}
		else
		{
			UpdateHomerunGauge(7);
			GoodImage->SetVisibility(ESlateVisibility::Visible);
			GameMode->AddScore(20);
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

		FVector ballloc = ball->GetActorLocation();

		Distance = FVector::Dist(ballloc, StrikeZoneLocation) / 30;
		FText WinConditionText = FText::FromString(TEXT("{0}FT"));
		FText WinCondition = FText::Format(WinConditionText, FText::AsNumber(Distance));
		HitDistanceText->SetText(WinCondition);
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
		//TODO: 불값 bSuccessfulCalledShot
		if (!bSuccessfulCalledShot)
		{
			if (ESlateVisibility::Visible != HomerunImage->GetVisibility() && ESlateVisibility::Visible != HitImage->
				GetVisibility())
			{
				isHomerunStateDisplaying = true;
				if (Homerun)
				{
					ComboNumber += 1;
					if (ComboNumber)
					{
						GameMode->AddScore(50);
					}
					else
					{
						GameMode->AddScore(20);
					}


					if (bCalledShot)
					{
						DisplayCyclingHomerun(CurrentDirection);
						DisplayCalledShotHomerun();
						
					}
					else // Display Homerun
					{
						DisplayCyclingHomerun(CurrentDirection);
						PlayAnimation(HomerunAnimation);
						HomerunImage->SetVisibility(ESlateVisibility::Visible);
						FTimerHandle HomerunTimer;
						GetWorld()->GetTimerManager().SetTimer(HomerunTimer, this, &UInGameUI::HideHomerunState,
						                                       DisplayTime, false);
						UpdateSuccessfulHomerun();
					}
				}
				else if (!bCalledShot) // Display Hit
				{
					ComboNumber = 0;
					PlayAnimation(HitAnimation);
					HitImage->SetVisibility(ESlateVisibility::Visible);
					FTimerHandle HitTimer;
					GetWorld()->GetTimerManager().SetTimer(HitTimer, this, &UInGameUI::HideHomerunState,
					                                       DisplayTime, false);
				}
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
	else if (isCalledShot)
	{
		GameMode->ChangeState(EGameModeState::CalledShot);
		// FTimerHandle ChangeStateTimer;
		// GetWorld()->GetTimerManager().SetTimer(ChangeStateTimer,
		//                                        [this]() {  }, 1,
		//                                        false);
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
	TWeakObjectPtr<UInGameUI> WeakThis = this;
	GetWorld()->GetTimerManager().SetTimer(StopOSTTimer,
	                                       [WeakThis]()
	                                       {
		                                       if (WeakThis.IsValid())
		                                       {
			                                       WeakThis->InGameOSTComponent->Stop();
		                                       }
	                                       },
	                                       2,
	                                       false);
	if (PlayClearOST.IsBound())
	{
		PlayClearOST.Broadcast();
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	APawn* ClearPawn = GetWorld()->SpawnActor<APawn>(StageClearPawn, SpawnLocation, FRotator(0, -90.f, 0), Params);
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
	TWeakObjectPtr<UInGameUI> WeakThis = this;
	GetWorld()->GetTimerManager().SetTimer(StopOSTTimer,
	                                       [WeakThis]()
	                                       {
		                                       if (WeakThis.IsValid())
		                                       {
			                                       WeakThis->InGameOSTComponent->Stop();
		                                       }
	                                       },
	                                       2,
	                                       false);
	if (PlayFailOST.IsBound())
	{
		PlayFailOST.Broadcast();
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	APawn* FailPawn = GetWorld()->SpawnActor<APawn>(StageFailPawn, SpawnLocation, FRotator(0, -90.f, 0), Params);
	if (FailPawn)
	{
		FailPawn->SetActorScale3D(FVector(3.0f, 3.0f, 3.0f));
	}
	// Play Camera Move !!
	GameMode->ChangeState(EGameModeState::End); // play UI after camera move
}

void UInGameUI::DisplayCyclingHomerun(float Timing)
{
	FString Direction;
	if (Timing>-1.f && Timing<-0.3f)
	{
		Direction = "Left";
	}
	else if (Timing>=-0.3f&& Timing<0.3f)
	{
		Direction = "Center";
	}
	else if (Timing>=0.3f&& Timing<1.f)
	{
		Direction = "Right";
	}
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
		GameMode->AddScore(500);
	}
}

void UInGameUI::DisplayCalledShotHomerun()
{
	bSuccessfulCalledShot = true;
	CalledShotImage->SetVisibility(ESlateVisibility::Visible);


	FTimerHandle HideCalledShotTimer;
	GetWorld()->GetTimerManager().SetTimer(HideCalledShotTimer, this, &UInGameUI::HideCalledShotHomerun, DisplayTime,
	                                       false);
	GameMode->AddScore(700);
}

void UInGameUI::HideCalledShotHomerun()
{
	CalledShotImage->SetVisibility(ESlateVisibility::Hidden);

	DeductRemainingBalls();
	DisplayCombo();
	if (nullptr != GameMode->Ball)
	{
		GameMode->Ball->Destroy(); // Destroy Ball
	}

	bCalledShot = false;

	GameMode->ChangeState(EGameModeState::Throw);
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

bool UInGameUI::CheckCondition(bool bisHomerun)
{
	if (bisHomerun || bCalledShot)
	{
		return false;
	}
	return true;
}
