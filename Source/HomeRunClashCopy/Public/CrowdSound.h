// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CrowdSound.generated.h"

class USoundWave;
class UAudioComponent;

UCLASS()
class HOMERUNCLASHCOPY_API ACrowdSound : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACrowdSound();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	// 오디오 컴포넌트
	UPROPERTY(EditAnywhere, Category="Audio")
	UAudioComponent* CrowdAudioComponent;

	UPROPERTY(EditAnywhere, Category="Audio")
	UAudioComponent* CheerAudioComponent;

	// 사운드
	UPROPERTY(EditAnywhere, Category="Audio")
	USoundWave* CrowdSound;

	UPROPERTY(EditAnywhere, Category="Audio")
	USoundWave* CheerSound;

	UPROPERTY(EditAnywhere, Category="Audio")
    FTimerHandle TimerHandle;
	
public:
	UFUNCTION(BlueprintCallable)
	void PlayRandomCheer();

	UFUNCTION()
	void PlayCrowdSound();

	UFUNCTION()
	void StopCrowdSound();
};
