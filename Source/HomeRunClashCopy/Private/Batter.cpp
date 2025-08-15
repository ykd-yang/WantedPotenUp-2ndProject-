// Fill out your copyright notice in the Description page of Project Settings.


#include "Batter.h"

#include "Ball.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "HitBox.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABatter::ABatter()
{
 
	PrimaryActorTick.bCanEverTick = true;

}

void ABatter::PlayAnimSwing()
{
	if (bCanSwing)
	{
		PlaySwingMontage();
	}
}

void ABatter::ApplySwing()
{
	if (BallActor != nullptr)
	{
		float Timing =MyHitBox->CheckTiming(BallActor);
		float Height = MyHitBox->CheckHeight(BallActor);
		float Side = MyHitBox->CheckSide(BallActor);
		MyHitBox-> ApplyHit(Timing,Height,Side,BallActor);
		UE_LOG(LogTemp, Warning, TEXT("Timing: %f, HeightBat: %f, SideBat: %f, Ball Address: %p"), Timing, Height, Side, BallActor);
	}
	
}

void ABatter::PlaySwingMontage()
{

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && SwingMontage)
	{
		AnimInstance->Montage_Play(SwingMontage, 1.0f); 
	}
	
}

void ABatter::SetBallActor(class ABall* ball)
{
	BallActor = ball;
}

// Called when the game starts or when spawned
void ABatter::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	UEnhancedInputLocalPlayerSubsystem* SubSy = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
	SubSy->AddMappingContext(playerContext,0);
}

// Called every frame
void ABatter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABatter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UEnhancedInputComponent* input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    
    if (input)
    {
    	input -> BindAction(IA_Swing,ETriggerEvent::Triggered,this,&ABatter::PlayAnimSwing);
    }

}

