// Fill out your copyright notice in the Description page of Project Settings.


#include "Pitcher.h"
#include "Ball.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
APitcher::APitcher()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UDataTable> PitchTypeDataTableObject(TEXT("/Game/Data/PitchTypeDataTable.PitchTypeDataTable"));
	if (PitchTypeDataTableObject.Succeeded())
	{
		PitchTypeDataTable = PitchTypeDataTableObject.Object;
	}
}

// Called when the game starts or when spawned
void APitcher::BeginPlay()
{
	Super::BeginPlay();
		
	PitcherSkeletal = GetComponentByClass<USkeletalMeshComponent>();

	//Test
	if (PitcherSkeletal != nullptr)
	{
		SpawnBall();
		IsThrow= true;
	}
}

// Called every frame
void APitcher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APitcher::SpawnBall()
{
	UWorld* World = GetWorld();
	if (World)
	{
		Ball = World->SpawnActor<ABall>();
		FAttachmentTransformRules AttachRule(
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld,
			EAttachmentRule::KeepWorld,
			true);
		Ball->AttachToComponent(PitcherSkeletal, AttachRule, FName("RightHandSocket"));
	}
}

FBallInfo APitcher::GetRandomBallInfo()
{
	TArray<FName> RowNames = PitchTypeDataTable->GetRowNames();
	int32 RandomIndex = FMath::RandRange(0, RowNames.Num() - 1);
	FName RandomRowName = RowNames[RandomIndex];

	static const FString ContextString(TEXT("GetRandomBallInfo"));
	FBallInfo* RandomBallInfo = PitchTypeDataTable->FindRow<FBallInfo>(RandomRowName, ContextString, true);

	return *RandomBallInfo;
}

void APitcher::ThrowBall()
{
	FDetachmentTransformRules DetachRule(
		EDetachmentRule::KeepWorld,
		EDetachmentRule::KeepWorld,
		EDetachmentRule::KeepWorld,
		true);
	
	Ball->DetachFromActor(DetachRule);
	Ball->Init(GetRandomBallInfo());
	IsThrow = false;
}
