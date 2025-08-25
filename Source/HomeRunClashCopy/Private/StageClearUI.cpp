// Fill out your copyright notice in the Description page of Project Settings.


#include "StageClearUI.h"

#include "BaseBallGameMode.h"
#include "InGameUI.h"
#include "Components/AudioComponent.h"

void UStageClearUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (!ClearOSTComponent)
	{
		ClearOSTComponent = NewObject<UAudioComponent>(this);
		ClearOSTComponent->bAutoActivate = false;
		ClearOSTComponent->SetSound(ClearOSTSound);
		ClearOSTComponent->RegisterComponentWithWorld(GetWorld());
	}
	
	ABaseBallGameMode* Gm =  Cast<ABaseBallGameMode>(GetWorld()->GetAuthGameMode());
	Gm->InGameUI->PlayFailOST.AddDynamic(this, &UStageClearUI::PlayClearOST);
}

void UStageClearUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UStageClearUI::PlayClearOST()
{
	ClearOSTComponent->Play();
}

