// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BallInfo.h"
#include "GameFramework/Actor.h"
#include "Ball.generated.h"

class UNiagaraComponent;

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

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBallInfo BallInfo;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* BallMesh;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* Trail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* DragCoefficientCurve;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ABaseBallGameMode* Gm;

private:
	UPROPERTY(EditAnywhere)
	bool IsMove = false;
	float MagnusCoeff = 0.003f;
	FVector Gravity = FVector(0.0f, 0.0f, -980.0f);
	FVector Velocity = FVector(0.0f, 0.0f, 0.0f);
	FVector StartLocation = FVector(0.0f, 0.0f, 0.0f);

	FVector PrevLocation = FVector(0.0f, 0.0f, 0.0f);
	FVector SimulLocation = FVector(0.0f, 0.0f, 0.0f);
	FVector SimulVelocity = FVector(0.0f, 0.0f, 0.0f);
	float ElapsedTime = 0.0f;
	bool IsSimulate = false;
	bool IsFall = false;

public:
	void Init(FBallInfo BI, FVector Location);
	void SetBallMove();
	void SetBallHit(FVector HitVelocity);
	
private:
	void CalculateGravity(FVector& Vel);
	void CalculateMagnusSimple(FVector& Vel);
	void UpdateLocation(float DeltaTime);
	void UpdateRotation(float DeltaTime);

	UFUNCTION()
	void OnSimulate(float ElapTime);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
			   UPrimitiveComponent* OtherComp, FVector NormalImpulse,
			   const FHitResult& Hit);
};
