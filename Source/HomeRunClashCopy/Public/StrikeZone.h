// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ball.h"
#include "GameFramework/Actor.h"
#include "StrikeZone.generated.h"

UCLASS()
class HOMERUNCLASHCOPY_API AStrikeZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStrikeZone();
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UStaticMeshComponent* MeshComp;
	UFUNCTION(BlueprintCallable)
	float GetRatioInStrikeZone(class ABall* Ball);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	

};
