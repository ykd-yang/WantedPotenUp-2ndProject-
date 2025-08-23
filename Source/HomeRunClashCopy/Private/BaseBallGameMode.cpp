// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseBallGameMode.h"

#include "HitBox.h"
#include "Pitcher.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "InGameUI.h"
#include "MainMenuUI.h"
#include "StageClearUI.h"
#include "StageFailUI.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Components/Image.h"


ABaseBallGameMode::ABaseBallGameMode()
{
	State = EGameModeState::None;

	PrimaryActorTick.bCanEverTick = true;
}

void ABaseBallGameMode::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	// Create Widget
	if (nullptr != MainMenuUIClass)
	{
		MainMenuUI = CreateWidget<UMainMenuUI>(GetWorld(), MainMenuUIClass);
		if (nullptr != MainMenuUI)
		{
			MainMenuUI->AddToViewport(); // Display Widget
		}
	}

	InputModeUIOnly.SetWidgetToFocus(nullptr);
	InputModeUIOnly.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->SetInputMode(InputModeUIOnly);

	// Casting Pitcher
	AActor* FoundPitcher = UGameplayStatics::GetActorOfClass(GetWorld(), APitcher::StaticClass());
	if (FoundPitcher)
	{
		Pitcher = Cast<APitcher>(FoundPitcher);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Pitcher is null"));
	}


	AActor* FoundStartCamera = UGameplayStatics::GetActorOfClass(GetWorld(), ACameraActor::StaticClass());
	if (FoundStartCamera)
	{
		StartCamera = Cast<ACameraActor>(FoundStartCamera);
	}
}

void ABaseBallGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	switch (State)
	{
	case EGameModeState::Start:

		OnStartTick(DeltaTime);
		break;
	case EGameModeState::Throw:
		OnThrowTick(DeltaTime);
		break;
	case EGameModeState::CalledShot:
		OnCalledShotTick(DeltaTime);
		break;
	case EGameModeState::BallHit:
		OnBallHitTick(DeltaTime);
		break;
	case EGameModeState::BallMiss:
		OnBallMissTick(DeltaTime);
		break;
	case EGameModeState::End:
		OnEndTick(DeltaTime);
		break;
	default:
		break;
	}
}

void ABaseBallGameMode::ChangeState(EGameModeState NewState)
{
	if (State == NewState)
		return;
	switch (State)
	{
	case EGameModeState::Start:
		OnStartExit();
		break;
	case EGameModeState::Throw:
		OnThrowExit();
		break;
	case EGameModeState::CalledShot:
		OnCalledShotExit();
		break;
	case EGameModeState::BallHit:
		OnBallHitExit();
		break;
	case EGameModeState::BallMiss:
		OnBallMissExit();
		break;
	case EGameModeState::End:
		OnEndExit();
		break;
	default:
		break;
	}

	State = NewState;

	switch (State)
	{
	case EGameModeState::Start:
		OnStartEnter();
		break;
	case EGameModeState::Throw:
		OnThrowEnter();
		break;
	case EGameModeState::CalledShot:
		OnCalledShotEnter();
		break;
	case EGameModeState::BallHit:
		OnBallHitEnter();
		break;
	case EGameModeState::BallMiss:
		OnBallMissEnter();
		break;
	case EGameModeState::End:
		OnEndEnter();
		break;
	default: break;
	}
}

FString ABaseBallGameMode::BallTypeToString(EBallType BT)
{
	switch (BT)
	{
	case EBallType::Straight: return TEXT("4FSB");
	case EBallType::Curve: return TEXT("CB");
	case EBallType::Slider: return TEXT("SL");
	case EBallType::Fork: return TEXT("FO");
	case EBallType::ChangeUp: return TEXT("CH");
	case EBallType::Knuckle: return TEXT("KN");
	default: return TEXT("Unknown");
	}
}

void ABaseBallGameMode::GiveBallToGameMode(ABall* NewBall)
{
	Ball = NewBall;
}

//On Tick
void ABaseBallGameMode::OnStartTick(float DeltaTime)
{
	ElapsedTime += DeltaTime;
	if (ElapsedTime >= 1.0f) // 1초 기다림
	{
		FVector CurrentLoc = StartCamera->GetActorLocation();
		FVector NewLoc = FMath::VInterpTo(CurrentLoc, TargetLocation, DeltaTime, 2.0f);
		StartCamera->SetActorLocation(NewLoc);

		if (FVector::Dist(TargetLocation, StartCamera->GetActorLocation()) < 4.9f)
		{
			FRotator CurrentRot = StartCamera->GetActorRotation();
			FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRotation, DeltaTime, 2.0f);
			StartCamera->SetActorRotation(NewRot);
		}
	}
}

void ABaseBallGameMode::OnThrowTick(float DeltaTime)
{
}

void ABaseBallGameMode::OnCalledShotTick(float DeltaTime)
{
}

void ABaseBallGameMode::OnBallHitTick(float DeltaTime)
{
	// 3. 공이 땅에 닿으면 비거리 결정
	// 4. 비거리 결정 후 타격상태 표시 
	// 5. 타격상태 사리지면 Main Mission 갱신, 남은 공갯수 차감
	// 6. 1초후 카메라 타자시점 원상복귀
	// 7. 바로 다시 Throw State
	//비거리 표시
	if (!didBallFall) // !!공이 배트 이외의 물체를 닿았냐
	{
		//InGameUI->UpdateBallDistance(Ball, PlayerController); // strikezone location, ball location length
	}
	else if (InGameUI->isJudgementDisplaying) // 만약 판정UI가 남아 있다면 사라진 후에 표시
	{
		UE_LOG(LogTemp, Warning, TEXT("isJudgementDisplaying"))
		InGameUI->HideBallDistance();
	}
	else if (!InGameUI->isHomerunStateDisplaying)
	{
		UE_LOG(LogTemp, Warning, TEXT("isHomerunStateDisplaying"))
		InGameUI->HideBallDistance();
	}
}

void ABaseBallGameMode::OnBallMissTick(float DeltaTime)
{
}

void ABaseBallGameMode::OnEndTick(float DeltaTime)
{
}


//On Enter
void ABaseBallGameMode::OnStartEnter()
{
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), AllWidgets, UUserWidget::StaticClass(), true);
	AllWidgets[0]->SetVisibility(ESlateVisibility::Visible);
	PlayerController->bShowMouseCursor = false;
	PlayerController->SetInputMode(InputModeGameOnly);
	// 1. 화면이 타자시점이 된다
	// 2. 줌이되고 ReadyUI 표시
	// 3. 카메라가 Pitch방향으로 숙인다
	// 4. GoUI 표시
	FTimerHandle DisplayReadyTimer;
	GetWorld()->GetTimerManager().SetTimer(DisplayReadyTimer, [this]
	{
		if (InGameUI)
		{
			InGameUI->DisplayReady();
		}
	}, 1, false);
	// 5. 몇초 뒤 Throw State
	FTimerHandle StartTimer;
	GetWorld()->GetTimerManager().SetTimer(StartTimer, [this]() { ChangeState(EGameModeState::Throw); }, 5.7f, false);
}

void ABaseBallGameMode::OnThrowEnter()
{
	if (nullptr != InGameUI)
	{
		InGameUI->isHomerunStateDisplaying = false;
		// 카메라 원상복귀
		SwitchToStartCamera(PlayerController);
		// 	1.공을 던진다 + 공의 정보를 가져온다
		Pitcher->ThrowTrigger();
	}
}

void ABaseBallGameMode::OnCalledShotEnter()
{
}

void ABaseBallGameMode::OnBallHitEnter()
{
	// 1. 타격시 성공
	// 2. 공의 정보, 타자에서!!공의 방향, 타자에서!!타격 판정, tick에서 비거리 표시
	//BallType = Ball->BallInfo.BallType;
	InGameUI->DisplayBallInfo("BallTypeToString(BallType)");;
}

void ABaseBallGameMode::OnBallMissEnter()
{
	InGameUI->ComboNumber = 0;
	InGameUI->DisplayCombo();
	// 1. 공이 지나간다
	// 2. 지나간 후 남은 공갯수 차감, 공의 정보, 공의 방향, Miss표시(먼저 사라짐)
	if (ESlateVisibility::Visible != InGameUI->MissImage->GetVisibility())
	{
		UE_LOG(LogTemp, Warning, TEXT("Miss"));
		InGameUI->DisplayMiss();
		InGameUI->DisplayBallHitDirection(-2);
		//BallType = Ball->BallInfo.BallType;
		InGameUI->DisplayBallInfo("BallTypeToString(BallType)"); // 3. 공의 정보와 방향이 사라지면  4. 다시 Throw State
		FTimerHandle MissTimer;
		GetWorld()->GetTimerManager().SetTimer(MissTimer, [this]() { ChangeState(EGameModeState::Throw); },
		                                       InGameUI->DisplayTime, false);
		InGameUI->DeductRemainingBalls();
	}
	// else if (ESlateVisibility::Visible == InGameUI->MissImage->GetVisibility())
	// {
	// 	
	// }
	else // 헛스윙 시, 바로 !!타자에서 Miss 표시
	{
		UE_LOG(LogTemp, Warning, TEXT("헛스윙 Miss"));
		InGameUI->DisplayBallHitDirection(-2);
		//BallType = Ball->BallInfo.BallType;
		InGameUI->DisplayBallInfo("BallTypeToString(BallType)"); // 3. 공의 정보와 방향이 사라지면  4. 다시 Throw State
		FTimerHandle MissTimer;
		GetWorld()->GetTimerManager().SetTimer(MissTimer, [this]() { ChangeState(EGameModeState::Throw); },
		                                       InGameUI->DisplayTime, false);
		InGameUI->DeductRemainingBalls();
	}
}

void ABaseBallGameMode::OnEndEnter()
{
	if (InGameUI->IsStageCleared) // 스테이지 클리어 시
	{
		// 기존 UI 제거
		if (InGameUI)
		{
			InGameUI->RemoveFromParent();
			InGameUI = nullptr;
		}
		// 새로운 StageClearUI 생성
		if (StageClearUIClass)
		{
			StageClearUI = CreateWidget<UStageClearUI>(GetWorld(), StageClearUIClass);
			if (StageClearUI)
			{
				StageClearUI->AddToViewport();
			}
		}
		// 마우스 커서 활성화
		if (PlayerController)
		{
			PlayerController->bShowMouseCursor = true; // 마우스 커서 보이게
			InputModeUIOnly.SetWidgetToFocus(nullptr);
			InputModeUIOnly.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputModeUIOnly); // UI 모드로 입력 전환
		}
	}
	else // 스테이지 실패 시
	{
		// 기존 UI 제거
		if (InGameUI)
		{
			InGameUI->RemoveFromParent();
			InGameUI = nullptr;
		}
		// 새로운 StageFailUI 생성
		if (StageFailUIClass)
		{
			StageFailUI = CreateWidget<UStageFailUI>(GetWorld(), StageFailUIClass);
			if (StageFailUI)
			{
				StageFailUI->AddToViewport();
			}
		}
		// 마우스 커서 활성화
		if (PlayerController)
		{
			PlayerController->bShowMouseCursor = true; // 마우스 커서 보이게
			InputModeUIOnly.SetWidgetToFocus(nullptr);
			InputModeUIOnly.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputModeUIOnly); // UI 모드로 입력 전환
		}
	}
}

//On Exit
void ABaseBallGameMode::OnStartExit()
{
	ElapsedTime = 0.f;
}

void ABaseBallGameMode::OnThrowExit()
{
}

void ABaseBallGameMode::OnCalledShotExit()
{
}

void ABaseBallGameMode::OnBallHitExit()
{
	didBallFall = false;
	InGameUI->bHidingBallDistance = false;
}

void ABaseBallGameMode::OnBallMissExit()
{
}

void ABaseBallGameMode::OnEndExit()
{
}

void ABaseBallGameMode::SwitchToStartCamera(APlayerController* pc)
{
	if (pc && StartCamera)
	{
		// 부드럽게 시점 전환 (0.5초 동안)                                                                                                                                                
		pc->SetViewTargetWithBlend(StartCamera, 0.5f);
	}
}


void ABaseBallGameMode::SwitchToMainMenuUI()
{
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), AllWidgets, UUserWidget::StaticClass(), true);

	// 기존 UI 제거
	if (StageClearUI == AllWidgets[1])
	{
		StageClearUI->RemoveFromParent();
		StageClearUI = nullptr;
	}
	if (StageFailUI == AllWidgets[1])
	{
		StageFailUI->RemoveFromParent();
		StageFailUI = nullptr;
	}

	// 새로운 MainMenuUI 생성
	if (MainMenuUIClass)
	{
		MainMenuUI = CreateWidget<UMainMenuUI>(GetWorld(), MainMenuUIClass);
		if (MainMenuUI)
		{
			MainMenuUI->AddToViewport();
		}
	}
}

void ABaseBallGameMode::SwitchToInGameUI()
{
	// 기존 UI 제거
	if (MainMenuUI)
	{
		MainMenuUI->RemoveFromParent();
		MainMenuUI = nullptr;
	}
	// 새로운 InGameUI 생성
	if (InGameUIClass)
	{
		InGameUI = CreateWidget<UInGameUI>(GetWorld(), InGameUIClass);
		if (InGameUI)
		{
			InGameUI->AddToViewport();
		}
	}
}
