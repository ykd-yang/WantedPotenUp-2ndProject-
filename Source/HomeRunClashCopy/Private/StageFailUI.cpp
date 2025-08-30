// Fill out your copyright notice in the Description page of Project Settings.


#include "StageFailUI.h"

#include "BaseBallGameMode.h"
#include "InGameUI.h"
#include "Components/AudioComponent.h"

void UStageFailUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (!FailOSTComponent)
	{
		FailOSTComponent = NewObject<UAudioComponent>(this);
		FailOSTComponent->bAutoActivate = false;
		FailOSTComponent->SetSound(FailOSTSound);
		FailOSTComponent->RegisterComponentWithWorld(GetWorld());
	}
	
	// InGameUI 불러서
	ABaseBallGameMode* Gm =  Cast<ABaseBallGameMode>(GetWorld()->GetAuthGameMode());
	//FPlayOST PlayFailOST 바인드
	Gm->InGameUI->PlayFailOST.AddDynamic(this, &UStageFailUI::PlayFailOST);
}

void UStageFailUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}



void UStageFailUI::PlayFailOST()
{
	// 실패 노래 재생
	FailOSTComponent->Play();
	FailOSTComponent->SetVolumeMultiplier(0.45);
}
