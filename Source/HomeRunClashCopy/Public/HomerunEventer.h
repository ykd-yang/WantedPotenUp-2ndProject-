// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HomerunEventer.generated.h"

class ABaseBallGameMode;

UCLASS()
class HOMERUNCLASHCOPY_API AHomerunEventer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHomerunEventer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Score = 225;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ABaseBallGameMode* Gm;

	UFUNCTION(BlueprintCallable)
	void ChangeToHomeRun();

	UFUNCTION(BlueprintCallable)
	void AddScoreToGM(int AddScore);
	
public:

	UFUNCTION(BlueprintImplementableEvent)
	void OnBallHit(FVector HitLoctaion);
};
