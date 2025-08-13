// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BallInfo.h"
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

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBallInfo BallInfo;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* BallMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* DragCoefficientCurve;

private:
	UPROPERTY(EditAnywhere)
	bool IsMove = false;
	float MagnusCoeff = 0.003f;
	FVector Gravity = FVector(0.0f, 0.0f, -980.0f);
	FVector Velocity = FVector(0.0f, 0.0f, 0.0f);

public:
	void Init(FBallInfo BI);
	void SetBallHit(FVector HitVelocity);
	
private:
	void CalculateGravity(float DeltaTime);
	void CalculateMagnusSimple(float DeltaTime);
	void UpdateLocation(float DeltaTime);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
			   UPrimitiveComponent* OtherComp, FVector NormalImpulse,
			   const FHitResult& Hit);
};
