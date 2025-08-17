
#pragma once


#include "CoreMinimal.h"
#include "Ball.h"
#include "GameFramework/GameModeBase.h"
#include "BallInfo.h"
#include "BaseBallGameMode.generated.h"


class UStageFailUI;
class UStageClearUI;
class UInGameUI;
class APitcher;


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

	// InGame UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UInGameUI> InGameUIClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UInGameUI> InGameUI;
	// StageClear UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UStageClearUI> StageClearUIClass;
	UPROPERTY()
	TObjectPtr<UStageClearUI> StageClearUI;
	// StageFail UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UStageFailUI> StageFailUIClass;
	UPROPERTY()
	TObjectPtr<UStageFailUI> StageFailUI;
	
	// EBallType to String
	UFUNCTION()
	FString BallTypeToString(EBallType BT);

	// 
	UFUNCTION()
	void GiveBallToGameMode(ABall* NewBall);
	
private:
	// Tick
	void OnStartTick();
	void OnThrowTick();
	void OnBallHitTick();
	void OnBallMissTick();
	void OnEndTick();

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


public:
	bool didBallFall = false;
	bool isHomerun;
	// hit: -1 ~ 1 or miss: -2
	float BatterHitDirection;
	
	// Variables for InGameUI
	int32 RemainingBalls = 11;
	int32 HomerunsForWin = 6; 
	
	ABall* Ball;
	EBallType BallType;
};
