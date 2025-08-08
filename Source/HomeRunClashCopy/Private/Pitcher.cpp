// Fill out your copyright notice in the Description page of Project Settings.


#include "Pitcher.h"
#include "Ball.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
APitcher::APitcher()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UDataTable> PitchTypeDataTableObject(TEXT("/Game/Data/DT_PitchType.DT_PitchType"));
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
		FAttachmentTransformRules AttachRule(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld,
			EAttachmentRule::KeepWorld, true);
		Ball->AttachToComponent(PitcherSkeletal, AttachRule, FName("RightHandSocket"));
	}
}

void APitcher::ThrowBall()
{
}

UScriptStruct* APitcher::GetRandomPitchType()
{
	if (PitchTypeDataTable)
	{
		TArray<FName> RowNames = PitchTypeDataTable->GetRowNames();
		if (RowNames.Num() > 0)
		{
			FName RandomRowName = RowNames[FMath::RandRange(0, RowNames.Num() - 1)];
			auto a = PitchTypeDataTable->FindRow<UScriptStruct>(RandomRowName, TEXT(""));
			return PitchTypeDataTable->FindRow<UScriptStruct>(RandomRowName, TEXT(""));
		}
	}

	return nullptr;
}

