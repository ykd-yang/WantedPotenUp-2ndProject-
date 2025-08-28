// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "CallHitObejct.generated.h"

enum class ENiagaraSystemInstanceState : uint8;

UCLASS()
class HOMERUNCLASHCOPY_API ACallHitObejct : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACallHitObejct();
	UPROPERTY(EditAnywhere,Category="FX",BlueprintReadWrite)
	UNiagaraSystem* DoorSystem;
	UPROPERTY(EditAnywhere,Category="FX",BlueprintReadWrite)
	UNiagaraComponent* Door;

	float CurrentRate=0.f;
	UPROPERTY(EditAnywhere)
	float TargetRate;
	void UpdateRate();
	UFUNCTION(BlueprintCallable)
	UNiagaraComponent* SpawnDoor();
	UNiagaraComponent* GetDoor();
	int LifeCount=0;
	UFUNCTION(BlueprintCallable)
	void DestroyDoor();
	UFUNCTION(BlueprintCallable)
	void SetDoor(UNiagaraComponent* NewDoor);
	FTimerHandle DoorTimerHandle;

	bool BisOnCallHit=false;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere,Category="FX",BlueprintReadWrite)
	class UBoxComponent* BoxComp;
		

};
