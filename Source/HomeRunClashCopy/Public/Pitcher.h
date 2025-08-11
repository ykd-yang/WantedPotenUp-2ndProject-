// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BallInfo.h"
#include "Engine/DataTable.h"
#include "Pitcher.generated.h"

class ABaseBallGameMode;
class ABall;

UCLASS()
class HOMERUNCLASHCOPY_API APitcher : public AActor
{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	APitcher();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<ABall> Ball;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* PitcherSkeletal;
	
	UPROPERTY(EditAnywhere, Category = "Data")
	UDataTable* PitchTypeDataTable;

	UPROPERTY(EditAnywhere)
	ABaseBallGameMode* Gm;
	

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsThrow = false;
	
private:
	FBallInfo GetRandomBallInfo();
	UFUNCTION(BlueprintCallable)
	void ThrowBall();

public:
	UFUNCTION(BlueprintCallable)
	void SpawnBall();
	
	UFUNCTION(BlueprintCallable)
	void ThrowTrigger();
};
