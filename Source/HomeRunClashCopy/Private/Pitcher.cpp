// Fill out your copyright notice in the Description page of Project Settings.


#include "Pitcher.h"
#include "Ball.h"
#include "BaseBallGameMode.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
APitcher::APitcher()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ConstructorHelpers::FObjectFinder<UDataTable> PitchTypeDataTableObject(TEXT("/Game/Data/PitchTypeDataTable.PitchTypeDataTable"));
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

	//get gamemode
	Gm = Cast<ABaseBallGameMode>(GetWorld()->GetAuthGameMode());
	if (Gm == nullptr)
	{	
		UE_LOG(LogTemp, Warning, TEXT("GameMode is null"));
	}

	//to spawn ball and set throw location
	ThrowTrigger();
}

// Called every frame
void APitcher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//for Test
	testT+=DeltaTime;
	if (testT > 5.f)	
	{
		ThrowTrigger();
		testT = 0.f;
	}
}

void APitcher::SpawnBall()
{
	Ball = GetWorld()->SpawnActor<ABall>(ABall::StaticClass());
	FAttachmentTransformRules AttachRule(
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepWorld,
		EAttachmentRule::KeepWorld,
		true);
	Ball->AttachToComponent(PitcherSkeletal, AttachRule, FName("RightHandSocket"));
	Ball->Init(GetRandomBallInfo(), ThrowLocation);
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
	if (ThrowLocation == FVector(0, 0, 0) || Ball == nullptr)
	{
		ThrowLocation = PitcherSkeletal->GetSocketLocation(FName("RightHandSocket"));
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *ThrowLocation.ToString());
		IsThrow = false;
		return;
	}
	
	FDetachmentTransformRules DetachRule(
		EDetachmentRule::KeepWorld,
		EDetachmentRule::KeepWorld,
		EDetachmentRule::KeepWorld,
		true);
	Ball->DetachFromActor(DetachRule);
	
	Ball->SetBallMove();
	IsThrow = false;
}

void APitcher::ThrowTrigger()
{
	if (ThrowLocation != FVector(0, 0, 0))
	{
		SpawnBall();
	}
	
	IsThrow = true;
}
