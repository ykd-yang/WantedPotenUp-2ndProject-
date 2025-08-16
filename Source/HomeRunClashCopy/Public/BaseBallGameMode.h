
#pragma once


#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BallInfo.h"
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
class UInGameUI;

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
	TSubclassOf<UInGameUI> InGameUI;
	UPROPERTY() 
	TObjectPtr<UInGameUI> InGameWidget;

	// EBallType to String
	UFUNCTION()
	FString BallTypeToString(EBallType BT);
	
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

	EBallType BallType;
};
