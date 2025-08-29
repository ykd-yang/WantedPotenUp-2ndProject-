
// Fill out your copyright notice in the Description page of Project Settings.


#include "ACallHitObject.h"

#include "Ball.h"
#include "BaseBallGameMode.h"
#include "InGameUI.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
ACallHitObejct::ACallHitObejct()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);
	

}

void ACallHitObejct::UpdateRate()
{
	LifeCount++;
}

UNiagaraComponent* ACallHitObejct::SpawnDoor()
{
	Door = UNiagaraFunctionLibrary::SpawnSystemAtLocation
	(GetWorld(),DoorSystem,GetActorLocation(),GetActorRotation(),FVector(0.f,10.f,7.f),false,true,ENCPoolMethod::None,true);
	if (Door)
	{
		Door->SetAutoDestroy(false);
		BisOnCallHit =true;
	}

	return Door;
}

UNiagaraComponent* ACallHitObejct::GetDoor()
{
	return Door;
}

void ACallHitObejct::DestroyDoor()
{
	if (Door)
	{
		Door->DeactivateImmediate();
		Door = nullptr;
		BisOnCallHit =false;
		LifeCount=0;
	}
	
}

void ACallHitObejct::SetDoor(UNiagaraComponent* NewDoor)
{
	Door = NewDoor;
}

// Called when the game starts or when spawned
void ACallHitObejct::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACallHitObejct::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (OtherActor->IsA(ABall::StaticClass()))
	{
		//TODO: 예고홈런이 켜져있음을 확인한다. perfect
		if (nullptr!= Door)
		{
			//TODO : 예고홈런존을 서서히 사라지게 한다.
			DestroyDoor();
			//TODO :  게임모드에 맞았음을 알린다.
			auto* MyGameMode = Cast<ABaseBallGameMode>(GetWorld()->GetAuthGameMode());
			UE_LOG(LogTemp,Warning,TEXT("아오"));
			MyGameMode-> InGameUI->bCalledShot = true;
		}
		
		
		
	}
}

// Called every frame
void ACallHitObejct::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}





