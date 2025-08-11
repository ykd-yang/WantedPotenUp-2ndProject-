// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BaseBallGameMode.generated.h"


enum class EGameModeState : uint8
{
	None		UMETA(DisplayName = "None"),
	Start		UMETA(DisplayName = "Start"),
	Throw		UMETA(DisplayName = "Throw"),
	BallHit		UMETA(DisplayName = "BallHit"),
	BallMiss	UMETA(DisplayName = "BallMiss"),
	End			UMETA(DisplayName = "End"),
};
/**
 * 
 */

class APitcher;

UCLASS()
class HOMERUNCLASHCOPY_API ABaseBallGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABaseBallGameMode();

private:
	EGameModeState State = EGameModeState::None;

	UPROPERTY(EditAnywhere)
	TObjectPtr<APitcher> Pitcher;

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	void ChangeState(EGameModeState NewState);

private:
	// Tick
	void OnStart();
	void OnThrow();
	void OnBallHit();
	void OnBallMiss();
	void OnEnd();

	//OnState Enter
	void OnStartEnter();
	void OnThrowEnter();
	void OnBallHitEnter();
	void OnBallMissEnter();
	void OnEndEnter();

	//OnState End
	void OnStartExit();
	void OnThrowExit();
	void OnBallHitExit();
	void OnBallMissExit();
	void OnEndExit();
};
