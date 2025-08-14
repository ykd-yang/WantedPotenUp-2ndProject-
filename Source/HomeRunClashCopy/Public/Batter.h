// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Batter.generated.h"

UCLASS()
class HOMERUNCLASHCOPY_API ABatter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABatter();
	UPROPERTY(EditAnywhere)
	class AHitBox* MyHitBox;
	UPROPERTY(EditAnywhere)
	class UInputAction* IA_Swing;
	UPROPERTY(EditAnywhere)
	class UInputMappingContext * playerContext;
	void PlayAnimSwing();
	void ApplySwing();
	bool bCanSwing = true;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	class UAnimMontage* SwingMontage;

	void PlaySwingMontage();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
