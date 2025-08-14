// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "HitBox.generated.h"


UCLASS()
class HOMERUNCLASHCOPY_API AHitBox : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AHitBox();
	
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* MyRootComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* CursorPlaneRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* CursorPlane;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* AimRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* HitBox;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UStaticMeshComponent* HitBoxMesh;

	FVector CamLoc;
	FRotator CamRot;
	FVector CamFwd;
	
	float DepthOffset;
	FVector Plane;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void InitCursorPlane();
	void RecognizeCursorInPlane();
	void ApplyHit(float Timing, float HeightBat, float SideBat, class ABall* ball);
	float CheckTiming(class ABall* Ball);
	float CheckHeight(class ABall* Ball);
	float CheckSide(class ABall* Ball);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
