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

	UPROPERTY(EditAnywhere)
	class AStrikeZone* StrikeZoneActor;

	FVector CamLoc;
	FRotator CamRot;
	FVector CamFwd;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UUserWidget> AimWidgetClass;
	UPROPERTY(EditAnywhere,Category = "UI")
	class UUserWidget* AimWidget;
	
	float DepthOffset;
	FVector Plane;

	UPROPERTY(EditAnywhere, Category = "Effects")
	class UParticleSystem* ExplosionEffect;

	float ItemPower=0.f;
	float ItemCritical=0.f;;
	

	float CheckTiming(class ABall* Ball);
	float CheckHeight(class ABall* Ball);
	bool CheckCritical(float Timing, float HeightBat, float SideBat);
	float CheckSide(class ABall* Ball);
	bool ApplyHit(float Timing, float HeightBat, float SideBat, class ABall* ball);
	bool ApplyHitReal(float Timing, float HeightBat, float SideBat, ABall* Ball);
	void SpawnEffect(const FVector& SpawnLocation);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void InitCursorPlane();
	void RecognizeCursorInPlane();
	
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
