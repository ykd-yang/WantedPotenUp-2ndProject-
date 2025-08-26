// Fill out your copyright notice in the Description page of Project Settings.


#include "CallShotNotfiy.h"

#include "BaseBallGameMode.h"
#include "InGameUI.h"

void UCallShotNotfiy::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	auto* myGameMode = Cast<ABaseBallGameMode>(GetWorld()->GetAuthGameMode());
	if (myGameMode)
	{
		myGameMode->ChangeState(EGameModeState::Throw);
		myGameMode -> InGameUI->ResetHomerunGauge();
	}
}
