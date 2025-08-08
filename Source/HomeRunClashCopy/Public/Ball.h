// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ball.generated.h"

UCLASS()
class HOMERUNCLASHCOPY_API ABall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	bool IsInit = false;
	float MagnusCoeff = 0.003f;
	FVector Gravity = FVector(0.0f, 0.0f, -980.0f);
	FVector Velocity = FVector(0.0f, 0.0f, 0.0f);

private:
	void CalculateGravity();
	void CalculateMagnusSimple();
};
