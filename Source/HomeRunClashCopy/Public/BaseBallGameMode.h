
#pragma once


#include "CoreMinimal.h"
#include "ACallHitObject.h"
#include "Ball.h"
#include "GameFramework/GameModeBase.h"
#include "BallInfo.h"
#include "BaseBallGameMode.generated.h"


class AHitBox;
class UMainMenuUI;
class UStageFailUI;
class UStageClearUI;
class UInGameUI;
class APitcher;

UENUM(BlueprintType)
enum class EGameModeState : uint8
{
	None		UMETA(DisplayName = "None"),
	Start		UMETA(DisplayName = "Start"),
	Throw		UMETA(DisplayName = "Throw"),
	CalledShot	UMETA(DisplayName = "CalledShot"),
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
	UFUNCTION(BlueprintCallable)
	void ChangeState(EGameModeState NewState);

	// StageFail UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UMainMenuUI> MainMenuUIClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UMainMenuUI> MainMenuUI;
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
	void OnStartTick(float DeltaTime);
	void OnThrowTick(float DeltaTime);
	void OnCalledShotTick(float DeltaTime);
	void OnBallHitTick(float DeltaTime);
	void OnBallMissTick(float DeltaTime);
	void OnEndTick(float DeltaTime);

	//OnState Enter
	void OnStartEnter();
	void OnThrowEnter();
	void OnCalledShotEnter();
	void OnBallHitEnter();
	void OnBallMissEnter();
	void OnEndEnter();

	//OnState End
	void OnStartExit();
	void OnThrowExit();
	void OnCalledShotExit();
	void OnBallHitExit();
	void OnBallMissExit();
	void OnEndExit();


public:
	bool didBallFall = false;
	bool isHomerun;
	// hit: -1 ~ 1 or miss: -2
	float BatterHitDirection;
	
	// Variables for InGameUI
	int32 MaxRemainingBalls = 12;
	int32 RemainingBalls = 12;
	int32 HomerunsForWin = 10; 

	UPROPERTY()
	ABall* Ball;
	EBallType BallType;

	UPROPERTY()
	ACameraActor* StartCamera;
	UPROPERTY()
	ACameraActor* EndCamera;
	FVector TargetLocation = FVector(3380.0f,-470.0f,280.0f);
	FRotator TargetRotation = FRotator(0.f,180.f,0.f);
	float ElapsedTime;
	UPROPERTY()
	APlayerController* PlayerController;
	FInputModeUIOnly InputModeUIOnly;
	FInputModeGameOnly InputModeGameOnly;
	UPROPERTY()
	TArray<UUserWidget*> AllWidgets;
	
	void SwitchToStartCamera(APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable)
	void SwitchToMainMenuUI();

	UFUNCTION(BlueprintCallable)
	void SwitchToInGameUI();

	int32 Score;
	UPROPERTY(VisibleAnywhere)
	TArray<class ACallHitObejct*> _CallHitPoints;
	void InitializeCallHitPoints();
	UFUNCTION(BlueprintCallable)
	TArray<ACallHitObejct*> GetCallHitPoints();
	void AddScore(int32 score);
};
