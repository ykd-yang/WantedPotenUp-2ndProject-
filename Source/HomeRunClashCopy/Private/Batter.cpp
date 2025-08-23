// Fill out your copyright notice in the Description page of Project Settings.


#include "Batter.h"

#include "Ball.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "HitBox.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

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

bool ABatter::ApplySwing()
{
	
	if (BallActor != nullptr)
	{
		BallActor->SetActorRotation(FRotator(0, 0, 0));
		float Timing =MyHitBox->CheckTiming(BallActor);
		float Height = MyHitBox->CheckHeight(BallActor);
		float Side = MyHitBox->CheckSide(BallActor);
		UE_LOG(LogTemp, Warning, TEXT("Timing: %f, HeightBat: %f, SideBat: %f, Ball Address: %p"), Timing, Height, Side, BallActor);
		return MyHitBox-> ApplyHit(Timing,Height,Side,BallActor);
		
	}
	return false;
}
bool ABatter::ApplySwingReal()
{
	
	if (BallActor != nullptr && MyHitBox != nullptr)
	{
		BallActor->SetActorRotation(FRotator(0, 0, 0));
		float Timing =MyHitBox->CheckTiming(BallActor);
		float Height = MyHitBox->CheckHeight(BallActor);
		float Side = MyHitBox->CheckSide(BallActor);
		bool Critical = MyHitBox->CheckCritical(Timing,Height,Side);
		UE_LOG(LogTemp, Warning, TEXT("Timing: %f, HeightBat: %f, SideBat: %f, Ball Address: %p"), Timing, Height, Side, BallActor);
		bool bIsSucced =  MyHitBox-> ApplyHitReal(Timing,Height,Side,BallActor);
		if (bIsSucced)
		{
			//OnHit.Broadcast(Timing, BallActor,Critical);  실제 플레이용
			OnHit.Broadcast(Timing, BallActor,bIsCritical); // 테스트용
			
			return true;
		}
		else
		{
			return false; 
		}
	}
	return false;
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

void ABatter::ForCriticalTest()
{
	bIsCritical  = !bIsCritical;
}


// Called when the game starts or when spawned
void ABatter::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	UEnhancedInputLocalPlayerSubsystem* SubSy = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
	SubSy->AddMappingContext(playerContext,0);
	if (AimWidgetClass)
	{
		
		AimWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), AimWidgetClass);

		
		if (AimWidgetInstance)
		{
			AimWidgetInstance->AddToViewport();
		}
	}
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
    	input -> BindAction(IA_Swing,ETriggerEvent::Triggered,this,&ABatter::PlaySwingMontage);
    	input -> BindAction(IA_Test , ETriggerEvent::Triggered,this,&ABatter::ForCriticalTest);
    }

}

