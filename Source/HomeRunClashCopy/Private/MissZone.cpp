// Fill out your copyright notice in the Description page of Project Settings.


#include "MissZone.h"

#include "Ball.h"
#include "BaseBallGameMode.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMissZone::AMissZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MissZone = CreateDefaultSubobject<UBoxComponent>(TEXT("MissZone"));
	SetRootComponent(MissZone);
}

// Called when the game starts or when spawned
void AMissZone::BeginPlay()
{
	Super::BeginPlay();

	
	MissZone->OnComponentBeginOverlap.AddDynamic(this, &AMissZone::OnOverlapMissZone);
}

// Called every frame
void AMissZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMissZone::OnOverlapMissZone(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->IsA(ABall::StaticClass()))
	{
		AGameModeBase* GameModeBase = UGameplayStatics::GetGameMode(this);
		ABaseBallGameMode* MyGameMode = Cast<ABaseBallGameMode>(GameModeBase);
		if (MyGameMode)
		{
			MyGameMode->ChangeState(EGameModeState::BallMiss);
		}
	}
}