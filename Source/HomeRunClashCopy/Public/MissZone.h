// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MissZone.generated.h"

class UBoxComponent;

UCLASS()
class HOMERUNCLASHCOPY_API AMissZone : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMissZone();
	// Collision Box
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* MissZone;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Begin Overlap
	UFUNCTION(BlueprintCallable)
	void OnOverlapMissZone(UPrimitiveComponent* OverlappedComp,
	                       AActor* OtherActor,
	                       UPrimitiveComponent* OtherComp,
	                       int32 OtherBodyIndex,
	                       bool bFromSweep,
	                       const FHitResult& SweepResult);
};
