// Fill out your copyright notice in the Description page of Project Settings.


#include "HomerunEventer.h"
#include "InGameUI.h"
#include "BaseBallGameMode.h"

// Sets default values
AHomerunEventer::AHomerunEventer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AHomerunEventer::BeginPlay()
{
	Super::BeginPlay();

	Gm = Cast<ABaseBallGameMode>(GetWorld()->GetAuthGameMode());
}

// Called every frame
void AHomerunEventer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHomerunEventer::ChangeToHomeRun()
{
	if (Gm == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("HomerunEventer Gm IS NULL"));
		return;
	}

	Gm->InGameUI->DisplayHomerunState(true);
}

void AHomerunEventer::AddScoreToGM(int AddScore)
{
	Gm->AddScore(AddScore);
}
