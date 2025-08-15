// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseBallGameMode.h"
#include "Pitcher.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

ABaseBallGameMode::ABaseBallGameMode()
{
	State = EGameModeState::None;
	
}

void ABaseBallGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Create Widget
	if (nullptr != UInGameUI)
	{
		GlobalWidget = CreateWidget<UUserWidget>(GetWorld(), UInGameUI);
		if (nullptr != GlobalWidget)
		{
			// Display Widget
			GlobalWidget->AddToViewport();
		}
	}
	
	AActor* FoundPitcher = UGameplayStatics::GetActorOfClass(GetWorld(), APitcher::StaticClass());
	if (FoundPitcher)
	{
		Pitcher = Cast<APitcher>(FoundPitcher);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Pitcher is null"));
	}
	
	ChangeState(EGameModeState::Start);
}

void ABaseBallGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	switch (State)
	{
		case EGameModeState::Start:
			OnStart();
			break;
		case EGameModeState::Throw:
			OnThrow();
			break;
		case EGameModeState::BallHit:
			OnBallHit();
			break;
		case EGameModeState::BallMiss:
			OnBallMiss();
			break;
		case EGameModeState::End:
			OnEnd();
			break;
		default:
			break;
	}
}

void ABaseBallGameMode::ChangeState(EGameModeState NewState)
{
	switch (State)
	{
	case EGameModeState::Start:
		OnStartExit();
		break;
	case EGameModeState::Throw:
		OnThrowExit();
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

//On Tick
void ABaseBallGameMode::OnStart()
{
}

void ABaseBallGameMode::OnThrow()
{
}

void ABaseBallGameMode::OnBallHit()
{
}

void ABaseBallGameMode::OnBallMiss()
{
}

void ABaseBallGameMode::OnEnd()
{
}


//On Enter
void ABaseBallGameMode::OnStartEnter()
{
}

void ABaseBallGameMode::OnThrowEnter()
{
	Pitcher->ThrowTrigger();
}

void ABaseBallGameMode::OnBallHitEnter()
{
}

void ABaseBallGameMode::OnBallMissEnter()
{
}

void ABaseBallGameMode::OnEndEnter()
{
}

//On Exit
void ABaseBallGameMode::OnStartExit()
{
}

void ABaseBallGameMode::OnThrowExit()
{
}

void ABaseBallGameMode::OnBallHitExit()
{
}

void ABaseBallGameMode::OnBallMissExit()
{
}

void ABaseBallGameMode::OnEndExit()
{
}
