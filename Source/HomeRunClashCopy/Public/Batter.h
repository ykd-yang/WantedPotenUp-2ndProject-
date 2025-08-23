// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Ball.h"
#include "Batter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHit,float,BaseBallDir,ABall*,HITBALL,bool, IsCritical);
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
	class UInputAction* IA_Test;
	UPROPERTY(EditAnywhere)
	class UInputMappingContext * playerContext;
	void PlayAnimSwing();
	UFUNCTION(BlueprintCallable)
	bool ApplySwing();
	UFUNCTION(BlueprintCallable)
	bool ApplySwingReal();
	bool bCanSwing = true;
	class ABall* BallActor;

	bool bIsCritical = false;
	

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bIsHiting =false;
	
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UUserWidget> AimWidgetClass;

	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UUserWidget* AimWidgetInstance;

	UPROPERTY(BlueprintAssignable)
	FOnHit OnHit;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	class UAnimMontage* SwingMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	class UAnimMontage* CallHitMontage;

	void PlaySwingMontage();
	void PlayCallHitMontage();
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void SetBallActor(class ABall* ball);
	 void ForCriticalTest();
	
	

};
